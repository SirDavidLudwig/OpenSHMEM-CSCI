#include "shmem.h"

// Library Setup/Querying --------------------------------------------------------------------------

int MY_PE;
int N_PES;

void shmem_init(int argc, char *argv[])
{
	// MPI init
	MPI_Init(&argc, &argv);

	// PE information
	MPI_Comm_rank(MPI_COMM_WORLD, &MY_PE);
	MPI_Comm_size(MPI_COMM_WORLD, &N_PES);

	// @TODO
	// Malloc shared memory
	shm_init(0);
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
	// Free shared memory
	shm_free();

	// Close MPI
	MPI_Finalize();
}

// Remote Memory Access ----------------------------------------------------------------------------

void shmem_get(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_get(dest, source, nelems, pe);
}

void shmem_put(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_put(dest, source, nelems, pe);
}

void shmem_get_nbi(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_get_nbi(dest, source, nelems, pe);
}

void shmem_put_nbi(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_put_nbi(dest, source, nelems, pe);
}

void shmem_getmem(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_getmem(dest, source, nelems, pe);
}

void shmem_putmem(char *dest, const char *source, size_t nelems, int pe)
{
	shmem_putmem(dest, source, nelems, pe);
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
