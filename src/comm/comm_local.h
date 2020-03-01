#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../memory/shared_heap.h"

/**
 * How shared memory is prefixed
 */
#define SHARED_MEMORY_PREFIX "dwl2x"

/**
 * The size of the symmetric heap
 */
#define SYMMETRIC_HEAP_SIZE 2 << 25

// Interface ---------------------------------------------------------------------------------------

/**
 * Initialize the local communication layer
 *
 * @param my_pe The current process' ID
 * @return      The shared heap object
 */
struct shared_heap_t* comm_local_init(int my_pe);

/**
 * Finalize the local communication layer
 *
 * @param n_pes         The number of processes
 * @param heap          A reference to the symmetric heap
 * @param shared_memory A list of shared memory address pointers
 */
void comm_local_finalize(int n_pes, struct shared_heap_t *heap, void **shared_memory);

/**
 * Wire up local processes
 *
 * @param my_pe   The current process' ID
 * @param n_pes   The number of processes
 * @param hostmap A map of hosts indexed by PE ID
 * @return        A list of shared memory pointers
 */
void** comm_local_wireup(int my_pe, int n_pes, char **hostmap);

// Communication Methods ---------------------------------------------------------------------------

/**
 * Get a value from a local PE
 *
 * @param heap The heap to get from
 * @param dest The destination variable to put the result
 * @param src  The source offset position within the heap
 * @param size The number of bytes to send
 */
void comm_local_get(void *heap, void *dest, long src, size_t size);

/**
 * Put a value into a local PE
 *
 * @param heap The heap to put to
 * @param dest The destination offset position within the heap
 * @param src  The source variable to send
 * @param size The number of bytes to send
 */
void comm_local_put(void *heap, long dest, const void *src, size_t size);
