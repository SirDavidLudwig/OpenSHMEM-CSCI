#include "work.h"
#include "../comm/comm.h"

#define _GNU_SOURCE

/**
 * The queue used for work
 */
static work_queue_t *__queue_head;
static work_queue_t *__queue_tail;

/**
 * A reference to the thread ID
 */
static pthread_t __thread_id;

/**
 * A mutex for thread-safe queue manipulation
 */
static pthread_mutex_t __lock;

/**
 * Indicate if the thread is finished
 */
static int __finished = 0;

/**
 * A thread-safe function to enqueue a network packet
 *
 * @param packet The packet to enqueue
 */
static void __work_thread_enqueue(work_job_t *job)
{
	pthread_mutex_lock(&__lock);
	if (__queue_head == NULL) {
		__queue_head = __queue_tail = malloc(sizeof(work_queue_t));
	} else {
		__queue_tail = __queue_tail->next = malloc(sizeof(work_queue_t));
	}
	printf("Enqueued: %ld\n", job->size);
	__queue_tail->job = job;
	__queue_tail->next = NULL;
	pthread_mutex_unlock(&__lock);
}

/**
 * A thread-safe function to dequeue a network packet
 *
 * @return A pointer to the dequeued packet if it exists; otherwise NULL
 */
static work_job_t* __work_thread_dequeue()
{
	work_queue_t *queued_job;
	work_job_t *result = NULL;

	pthread_mutex_lock(&__lock);
	if (NULL != (queued_job = __queue_head)) {
		printf("Dequeued a job task\n");
		result = queued_job->job;
		__queue_head = queued_job->next;
		// free(queued_job);
	}
	pthread_mutex_unlock(&__lock);
	return result;
}

/**
 * Execute a job on the worker thread
 *
 * @param job The job to execute
 */
void __work_thread_execute(work_job_t *job)
{
	printf("Performing a job... %ld\n", job->size);
	if (job->handler == HANDLER_PUT) {
		comm_put(job->dest, job->src, job->size, job->dest_pe);
	} else if (job->handler == HANDLER_GET) {
		// comm_get(job->dest, job->src, job->size, job->dest_pe);
	}
}

/**
 * The main worker thread
 */
void* __work_thread_run(void *arg)
{
	work_job_t *job;
	while (!__finished) {
		if (NULL != (job = __work_thread_dequeue())) {
			__work_thread_execute(job);
		}else {
			pthread_yield();
		}
	}
	return NULL;
}

/**
 * Start the worker thread
 */
void work_start()
{
	__queue_head = __queue_tail = NULL;
	__finished = 0;

	pthread_mutex_init(&__lock, NULL);
	pthread_create(&__thread_id, NULL, __work_thread_run, NULL);
}

/**
 * Finalize the worker thread
 */
void work_finalize()
{
	__finished = 1;
	pthread_join(__thread_id, NULL);
	pthread_mutex_destroy(&__lock);
}

/**
 * Insert a remote memory access operation into the worker queue
 *
 * @param handler The handler to process the active message
 * @param pe      The destination PE
 * @param dest    The destination location in the symmetric heap
 * @param src     The location of the data to put
 * @param size    The size of the data to put (in bytes)
 * @return        The request ID
 */
int work_put(char handler, int pe, void *dest, const void *src, size_t size)
{
	work_job_t *job;

	printf("Putting a job request\n");

	// Create the job descriptor
	job = malloc(sizeof(work_job_t));
	job->handler = handler;
	job->dest_pe = pe;
	job->origin_pe = comm_remote_pe();
	job->dest = dest;
	job->src = src;
	job->size = size;

	// Enqueue the job
	__work_thread_enqueue(job);

	return 0;
}

/**
 * Insert a remote memory access operation into the working memory queue on behalf of a remote PE
 *
 * @param handler   The handler to process the active message
 * @param pe        The destination PE
 * @param origin_pe The PE from which the operation originates
 * @param dest      The destination location in the symmetric heap
 * @param src       The location of the data to put
 * @param size      The size of the data to put (in bytes)
 * @return
 */
int work_put_remote(char handler, int pe, int origin_pe, void *dest, const void *src, size_t size)
{
	work_job_t *job;

	printf("Putting a remote job request: %ld\n", size);

	// Create the job descriptor
	job = malloc(sizeof(work_job_t));
	job->handler = handler;
	job->dest_pe = pe;
	job->origin_pe = origin_pe;
	job->dest = dest;
	job->src = src;
	job->size = size;

	printf("The value of the queue: %d\n", *((int*)job->src));

	// Enqueue the job
	__work_thread_enqueue(job);

	return 0;
}
