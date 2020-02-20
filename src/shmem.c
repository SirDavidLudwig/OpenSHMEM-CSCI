#include "shmem.h"

// Library Setup/Querying --------------------------------------------------------------------------

int MY_PE;
int N_PES;

void shmem_init()
{
	// MPI init
	MPI_Init(NULL, NULL);

	// PE information
	MPI_Comm_rank(MPI_COMM_WORLD, &MY_PE);
	MPI_Comm_size(MPI_COMM_WORLD, &N_PES);

	// Malloc shared memory
	shm_init(MY_PE);

	// Create worker thread
	worker_init();
}

int shmem_my_pe()
{
	return MY_PE;
}

int shmem_n_pes()
{
	return N_PES;
}

void shmem_finalize()
{
	// Shutdown the worker thread
	worker_finalize();

	// Free shared memory
	shm_free();

	// Close MPI
	MPI_Finalize();
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
	//
}

void shmem_putmem(void *dest, const void *source, size_t nelems, int pe)
{
	//
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
	//
}

void shmem_sync_all()
{
	//
}

// Memory Management -------------------------------------------------------------------------------

void *shmem_malloc(size_t size)
{
	return shm_malloc(size);
}

// Memory Ordering ---------------------------------------------------------------------------------

void shmem_quiet()
{
	//
}
