#include "shmem.h"

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
	comm_init(shmem_my_pe());

	// Create worker thread
	worker_init();
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

void shmem_barrier_all()
{
	// TEMPORARY
	sleep(1);
}

void shmem_sync_all()
{
	//
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
