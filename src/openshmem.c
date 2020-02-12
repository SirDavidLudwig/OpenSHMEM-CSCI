#include "openshmem.h"

// Library Setup/Querying --------------------------------------------------------------------------

/**
 * The Symmetric Heap
 */
void *symmetric_heap;

void shmem_init()
{
	// Create the symmetric heap
	symmetric_heap = malloc(4096);
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
	shmem_get(SHMEM_CTX_DEFAULT, dest, source, nelems, pe);
}

void shmem_get(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_put(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	shmem_put(SHMEM_CTX_DEFAULT, dest, source, nelems, pe);
}

void shmem_put(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	shmem_putmem(ctx, dest, source, nelems, pe);
}

void shmem_get_nbi(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	shmem_get_nbi(SHMEM_CTX_DEFAULT, dest, source, nelems, pe);
}

void shmem_get_nbi(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_put_nbi(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	shmem_put_nbi(SHMEM_CTX_DEFAULT, dest, source, nelems, pe);
}

void shmem_put_nbi(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_getmem(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	shmem_getmem(SHMEM_CTX_DEFAULT, dest, source, nelems, pe);
}

void shmem_getmem(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	//
}

void shmem_putmem(TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	shmem_putmem(SHMEM_CTX_DEFAULT, dest, source, nelems, pe);
}

void shmem_putmem(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe)
{
	comm_send(dest, source, nelems, pe);
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
	static heap_ptr = 0;

	// Allocate memory for a remotely accessible symmetric data object
	void* addr = symmetric_heap + heap_ptr;

	// Increment the heap ptr
	heap_ptr += size;

	// Return the final address
	return addr;
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

// TEMP --------------------------------------------------------------------------------------------

/**
 * @TODO Remove
 * A temporary function to demonstrate memory allocation on the symmetric heap
 */
int my_function()
{
	// Allocate the memory for a variable
	int *a = shmem_malloc(sizeof(int));

	// Assign a value to the variable
	*a = 10;
}
