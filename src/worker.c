#include "worker.h"

// Private Data Members ----------------------------------------------------------------------------

/**
 * A reference to the worker thread
 */
static pthread_t __thread_id;

/**
 * Indicate if the thread has been finalized
 */
static int __finalized;

/**
 * Indicate if the worker thread is ready
 */
static int __ready;

static void *__task_queue_head;
static void *__task_queue_tail;

static void *__completed_queue_head;
static void *__completed_queue_tail;

// Thread ------------------------------------------------------------------------------------------

void __dequeue()
{

}

/**
 * Run the worker thread
 */
void __worker_run()
{

}

void* __worker_thread(void* args)
{
	// Initialize the queues
	__task_queue_head      = NULL;
	__task_queue_tail      = NULL;
	__completed_queue_head = NULL;
	__completed_queue_tail = NULL;

	__worker_run();

	// Set the worker ready
	__ready = 1;
	return 0;
}

// Layer Management --------------------------------------------------------------------------------

/**
 * Create and initialize the worker thread
 */
void worker_init()
{
	__ready = 0;
	__finalized = 0;
	pthread_create(&__thread_id, NULL, __worker_thread, NULL);
}

/**
 * Check if the worker thread is ready
 */
int worker_is_ready()
{
	return __ready == 1;
}

/**
 * Clean up and detsroy the worker thread
 */
void worker_finalize()
{
	__ready = 0;
	__finalized = 1;
	pthread_join(__thread_id, NULL);
}

// Interface ---------------------------------------------------------------------------------------

/**
 * Insert a request into the queue
 */
void enqueue()
{

}

/**
 * Dequeue a completed task
 */
void dequeue()
{

}
