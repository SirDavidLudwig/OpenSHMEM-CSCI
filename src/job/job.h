#pragma once

#include "string.h"
#include "pthread.h"

/**
 * A container for a job's information
 */
typedef struct job {
	void *args;
	pthread_t thread_id;
} job_t;

/**
 * Create a new job
 *
 * @param handler The sub-routine to run in a separate thread
 * @param args    The list of arguments to give to the sub-routine
 */
job_t* job_start(void *(*handler)(void*), void *args);

/**
 * Wait for the given job to finish
 *
 * @param job The job to wait on
 * @return    The result of the job handler
 */
void* job_wait(job_t *job);

/**
 * Terminate a job immediately
 *
 * @param job The job to terminate
 */
void job_terminate(job_t *job);

/**
 * Free a job's information from memory
 *
 * @param job The job to free
 */
void job_free(job_t *job);
