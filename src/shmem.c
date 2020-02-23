#include "shmem.h"

/**
 * The default value for the pSync array
 */
#define SHMEM_SYNC_VALUE -1L

// Private Data Members ----------------------------------------------------------------------------

/**
 * The pSync array used for barriers
 */
long *__pSync;

// Internal Functions ------------------------------------------------------------------------------

/**
 * Initialize the pSync array used for barriers
 */
void __shmem_init_psync()
{
	__pSync = shared_memory_malloc(shmem_n_pes() * sizeof(long));
	for (int i = 0; i < shmem_n_pes(); i++) {
		__pSync[i] = SHMEM_SYNC_VALUE;
	}
}

/**
 * Post initialization tasks
 */
void __shmem_post_init()
{
	// pSync arrays for barriers
	__shmem_init_psync();

	// Wait for all processes to finish
	shmem_barrier_all();
}

// Library Setup/Querying --------------------------------------------------------------------------

void shmem_init()
{
	// Pre-initialize the runtime layer
	rte_preinit();

	// Malloc shared memory
	shared_memory_init(shmem_my_pe());

	// Initialize the runtime layer
	rte_init();

	// Create the communication thread
	comm_init();

	// Create worker thread
	worker_init();

	// Post initialization
	__shmem_post_init();
}

int shmem_my_pe()
{
	return rte_my_pe();
}

int shmem_n_pes()
{
	return rte_n_pes();
}

void shmem_finalize()
{
	// Shutdown the worker thread
	worker_finalize();

	comm_finalize();

	// Free shared memory
	shared_memory_free();

	// Close MPI
	rte_finalize();
}

// Remote Memory Access ----------------------------------------------------------------------------

void shmem_get(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_getmem(dest, source, nelems * sizeof(char), pe);
}

void shmem_put(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_putmem(dest, source, nelems * sizeof(char), pe);
}

void shmem_get_nbi(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_getmem_nbi(dest, source, nelems * sizeof(char), pe);
}

void shmem_put_nbi(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_putmem_nbi(dest, source, nelems * sizeof(char), pe);
}

void shmem_getmem(void *dest, const void *source, size_t nelems, int pe)
{
	comm_get(dest, shared_memory_offset(source), nelems, pe);
}

void shmem_putmem(void *dest, const void *source, size_t nelems, int pe)
{
	comm_put(shared_memory_offset(dest), source, nelems, pe);
}

void shmem_getmem_nbi(void *dest, const void *source, size_t nelems, int pe)
{
	//
}

void shmem_putmem_nbi(void *dest, const void *source, size_t nelems, int pe)
{
	//
}

// Collectives -------------------------------------------------------------------------------------

/**
 * @todo TEMP
 */
void shmem_barrier_all()
{
	MPI_Barrier(MPI_COMM_WORLD);
	// int pe = shmem_my_pe();
	// int n_pes = shmem_n_pes();

	// // Increment this process' pSync value
	// __pSync[pe]++;

	// // Wait for other processes to update
	// for (int i = 0; i < n_pes; i++) {
	// 	while (__pSync[i] != __pSync[pe]);
	// }
}

/**
 * @todo TEMP
 */
void shmem_sync_all()
{
	shmem_barrier_all();
}

// Memory Management -------------------------------------------------------------------------------

void *shmem_malloc(size_t size)
{
	return shared_memory_malloc(size);
}

// Memory Ordering ---------------------------------------------------------------------------------

void shmem_quiet()
{
	// Wait for the worker thread to empty the completion queue
}
