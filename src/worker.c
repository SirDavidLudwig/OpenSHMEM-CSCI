#include "worker.h"

// Private Data Members ----------------------------------------------------------------------------

/**
 * A reference to the worker thread
 */
pthread_t __worker_thread_id;

/**
 * indicate if the thread has been finalized
 */
int __worker_finalized;

// Thread ------------------------------------------------------------------------------------------

void* __worker_thread(void* args)
{
	return 0;
}

// Layer Management --------------------------------------------------------------------------------

/**
 * Create and initialize the worker thread
 */
void worker_init()
{
	__worker_finalized = 0;
	pthread_create(&__worker_thread_id, NULL, __worker_thread, NULL);
}

/**
 * Clean up and detsroy the worker thread
 */
void worker_finalize()
{
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
