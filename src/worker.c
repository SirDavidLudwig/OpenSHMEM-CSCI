#include "worker.h"

// Private Data Members ----------------------------------------------------------------------------

/**
 * A reference to the worker thread
 */
pthread_t __worker_thread_id;

/**
 * Indicate if the thread has been finalized
 */
int __worker_finalized;

/**
 * Indicate if the worker thread is ready
 */
int __worker_ready;

// Thread ------------------------------------------------------------------------------------------

void* __worker_thread(void* args)
{
	// Set the worker ready
	__worker_ready = 1;
	return 0;
}

// Layer Management --------------------------------------------------------------------------------

/**
 * Create and initialize the worker thread
 */
void worker_init()
{
	__worker_ready = 0;
	__worker_finalized = 0;
	pthread_create(&__worker_thread_id, NULL, __worker_thread, NULL);
}

/**
 * Check if the worker thread is ready
 */
int worker_is_ready()
{
	return __worker_ready == 1;
}

/**
 * Clean up and detsroy the worker thread
 */
void worker_finalize()
{
	__worker_ready = 0;
	__worker_finalized = 1;
	pthread_join(__worker_thread_id, NULL);
}

// Interface ---------------------------------------------------------------------------------------

/**
 * Insert a request into the queue
 */
void enqueue()
{

}
