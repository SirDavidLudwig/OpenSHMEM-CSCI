#include "job.h"

/**
 * Create a new job
 *
 * @param handler The sub-routine to run in a separate thread
 * @param args    The list of arguments to give to the sub-routine
 */
job_t* job_start(void *(*handler)(void*), void *args)
{
	job_t *job = malloc(sizeof(job_t));
	pthread_create(&job->thread_id, NULL, handler, args);
	return job;
}

/**
 * Wait for the given job to finish
 *
 * @param job The job to wait on
 */
void* job_wait(job_t *job)
{
	void* result;
	pthread_join(job->thread_id, &result);
	return result;
}

/**
 * Terminate a job immediately
 *
 * @param job The job to terminate
 */
void job_terminate(job_t *job)
{
	pthread_cancel(job->thread_id);
}

/**
 * Free a job's information from memory
 *
 * @param job The job to free
 */
void job_free(job_t *job)
{
	free(job);
}
