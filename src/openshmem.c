#include "openshmem.h"

// Library Setup/Querying --------------------------------------------------------------------------

void shmem_init()
{
	//
}

int shmem_my_pe()
{
	//
}

int shmem_n_pes()
{
	//
}

void shmem_finalize()
{
	//
}

// Remote Memory Access ----------------------------------------------------------------------------

void shmem_get(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_get(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_put(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_put(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_get_nbi(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_get_nbi(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_put_nbi(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_put_nbi(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_getmem(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_getmem(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_putmem(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_putmem(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
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
	/* Generate Key */

	// Allocate from the symmetric heap
	void* ptr = shm_malloc(size);

	// Barrier

	return ptr;
}

// Memory Ordering ---------------------------------------------------------------------------------

void shmem_quiet()
{
	//
}

void shmem_ctx_quiet(shmem_ctx_t ctx)
{
	//
}
