#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
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

#define SHMEM_OP_SIGNATURE(type, typename, op, suffix)                                           \
	void shmem_##typename##_##op##suffix(type *dest, const type *source, size_t nelems, int pe);

#define SHMEM_OP_SIGNATURES(type, typename)       \
	SHMEM_OP_SIGNATURE(type, typename, get, )     \
	SHMEM_OP_SIGNATURE(type, typename, put, )     \
	SHMEM_OP_SIGNATURE(type, typename, get, _nbi) \
	SHMEM_OP_SIGNATURE(type, typename, put, _nbi)

SHMEM_OP_SIGNATURES(float, float)
SHMEM_OP_SIGNATURES(double, double)
SHMEM_OP_SIGNATURES(long double, longdouble)
SHMEM_OP_SIGNATURES(char, char)
SHMEM_OP_SIGNATURES(signed char, schar)
SHMEM_OP_SIGNATURES(short, short)
SHMEM_OP_SIGNATURES(int, int)
SHMEM_OP_SIGNATURES(long, long)
SHMEM_OP_SIGNATURES(long long, longlong)
SHMEM_OP_SIGNATURES(unsigned char, uchar)
SHMEM_OP_SIGNATURES(unsigned short, ushort)
SHMEM_OP_SIGNATURES(unsigned int, uint)
SHMEM_OP_SIGNATURES(unsigned long, ulong)
SHMEM_OP_SIGNATURES(unsigned long long, ulonglong)
SHMEM_OP_SIGNATURES(int8_t, int8)
SHMEM_OP_SIGNATURES(int16_t, int16)
SHMEM_OP_SIGNATURES(int32_t, int32)
SHMEM_OP_SIGNATURES(int64_t, int64)
SHMEM_OP_SIGNATURES(uint8_t, uint8)
SHMEM_OP_SIGNATURES(uint16_t, uint16)
SHMEM_OP_SIGNATURES(uint32_t, uint32)
SHMEM_OP_SIGNATURES(uint64_t, uint64)
SHMEM_OP_SIGNATURES(size_t, size)
SHMEM_OP_SIGNATURES(ptrdiff_t, ptrdiff)

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

/**
 * [Collective]
 * Free up a block of memory
 */
void shmem_free(void *ptr);

// Memory Ordering ---------------------------------------------------------------------------------

/**
 * Ensures completion of Put, AMO, memory store, and nonblocking Put and Get routines
 */
void shmem_quiet();
