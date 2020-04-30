#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define HANDLER_PUT 0
#define HANDLER_GET 1

/**
 * A single job to perform
 */
typedef struct work_job {
	char handler;
	int dest_pe;
	int origin_pe;
	void *dest;
	const void *src;
	size_t size;
} work_job_t;

/**
 * A container for jobs used for queues
 */
typedef struct work_queue {
	work_job_t *job;
	struct work_queue *next;
} work_queue_t;

/**
 * Start the worker thread
 */
void work_start();

/**
 * Finalize the worker thread
 */
void work_finalize();

/**
 * Send the given data to another PE
 *
 * @param pe   The destination PE
 * @param dest The destination location in the symmetric heap
 * @param src  The location of the data to put
 * @param size The size of the data to put (in bytes)
 * @return     The request ID
 */
int work_put(char handler, int pe, void *dest, const void *src, size_t size);

/**
 * Perform a put operation on the remote side.
 *
 * @param handler   The handler to process the active message
 * @param dest_pe   The destination PE
 * @param origin_pe The PE from which the operation originates
 * @param dest      The destination location in the symmetric heap
 * @param src       The location of the data to put
 * @param size      The size of the data to put (in bytes)
 * @return
 */
int work_put_remote(char handler, int pe, int origin_pe, void *dest, const void *src, size_t size);
