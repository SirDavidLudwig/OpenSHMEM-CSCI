#pragma once

#include <stdlib.h>

// Definitions -------------------------------------------------------------------------------------

/**
 * The default value for the pSync array
 */
#define SHMEM_SYNC_VALUE -1L

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
 * [Collective]
 * Release all resources used by OpenSHMEM
 */
void shmem_finalize();

// Remote Memory Access ----------------------------------------------------------------------------

/**
 * Copy data from the specified PE
 */
void shmem_get(char *dest, const char *source, size_t nelems, int pe);

/**
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_put(char *dest, const char *source, size_t nelems, int pe);

/**
 * [Nonblocking]
 * Copy data from the specified PE
 */
void shmem_get_nbi(char *dest, const char *source, size_t nelems, int pe);

/**
 * [Nonblocking]
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_put_nbi(char *dest, const char *source, size_t nelems, int pe);

/**
 * Copy data from the specified PE
 */
void shmem_getmem(void *dest, const void *source, size_t nelems, int pe);

/**
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_putmem(void *dest, const void *source, size_t nelems, int pe);

/**
 * [Nonblocking]
 * Copy data from the specified PE
 */
void shmem_getmem_nbi(void *dest, const void *source, size_t nelems, int pe);

/**
 * [Nonblocking]
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_putmem_nbi(void *dest, const void *source, size_t nelems, int pe);

// Collectives -------------------------------------------------------------------------------------

/**
 * [Collective]
 * Blocks the PE until all other PEs arrive at the barrier
 */
void shmem_barrier_all();

/**
 * [Collective]
 * Barrier and flush all current requests
 */
void shmem_sync_all();

// Memory Management -------------------------------------------------------------------------------

/**
 * [Collective]
 * Returns a pointer to a block of at least `size` bytes suitably aligned for any use
 */
void* shmem_malloc(size_t size);

// Memory Ordering ---------------------------------------------------------------------------------

/**
 * Ensures completion of Put, AMO, memory store, and nonblocking Put and Get routines
 */
void shmem_quiet();
