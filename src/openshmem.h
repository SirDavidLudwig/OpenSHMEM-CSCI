#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmem.h"

// Library Setup/Querying --------------------------------------------------------------------------

/**
 * [Collective]
 * Allocates and initializes resources used by OpenSHMEM
 */
void shmem_init();

/**
 * Returns the PE number for the calling PE
 */
int shmem_my_pe();

/**
 * Get the number of PEs running in a program
 */
int shmem_n_pes();

/**
 * Release all resources used by OpenSHMEM
 */
void shmem_finalize();

// Remote Memory Access ----------------------------------------------------------------------------

/**
 * Copy data from the specified PE
 */
void shmem_get(TYPE *dest, const TYPE *source, size_t nelems, int pe);
void shmem_get(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe);

/**
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_put(TYPE *dest, const TYPE *source, size_t nelems, int pe);
void shmem_put(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe);

/**
 * [Nonblocking]
 * Copy data from the specified PE
 */
void shmem_get_nbi(TYPE *dest, const TYPE *source, size_t nelems, int pe);
void shmem_get_nbi(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe);

/**
 * [Nonblocking]
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_put_nbi(TYPE *dest, const TYPE *source, size_t nelems, int pe);
void shmem_put_nbi(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe);

/**
 * Copy data from the specified PE
 */
void shmem_getmem(TYPE *dest, const TYPE *source, size_t nelems, int pe);
void shmem_getmem(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe);

/**
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_putmem(TYPE *dest, const TYPE *source, size_t nelems, int pe);
void shmem_putmem(shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe);

// Collectives -------------------------------------------------------------------------------------

/**
 * [Collective]
 * Blocks the PE until all other PEs arrive at the barrier
 */
void shmem_barrier_all();

/**
 * [Collective]
 * Suspends PE execution untill all other PEs arrive at the barrier
 */
void shmem_sync_all();

// Memory Management -------------------------------------------------------------------------------

/**
 * [Collective]
 * Returns a pointer to a block of at least `size` bytes suitably aligned for any use
 */
void *shmem_malloc(size_t size);

// Memory Ordering ---------------------------------------------------------------------------------

/**
 * Ensures completion of Put, AMO, memory store, and nonblocking Put and Get routines
 */
void shmem_quiet();
void shmem_ctx_quiet(shmem_ctx_t ctx);
