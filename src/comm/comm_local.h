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
 */
void comm_local_init(int my_pe);

/**
 * Finalize the local communication layer
 *
 * @param n_pes The number of processes
 */
void comm_local_finalize(int n_pes);

/**
 * Wire up local processes
 *
 * @param my_pe   The current process' ID
 * @param n_pes   The number of processes
 * @param hostmap A map of hosts indexed by PE ID
 */
void comm_local_wireup(int my_pe, int n_pes, char **hostmap);

// Communication Methods ---------------------------------------------------------------------------

/**
 * Get a value from a local PE
 *
 * @param pe   The PE to communicate with
 * @param dest The destination variable to put the result
 * @param src  The source offset position within the heap
 * @param size The number of bytes to send
 */
void comm_local_get(int pe, void *dest, long src, size_t size);

/**
 * Put a value into a local PE
 *
 * @param pe   The PE to communicate with
 * @param dest The destination offset position within the heap
 * @param src  The source variable to send
 * @param size The number of bytes to send
 */
void comm_local_put(int pe, long dest, const void *src, size_t size);

// Accessors ---------------------------------------------------------------------------------------

/**
 * Get a reference to the symmetric heap
 *
 * @return A reference to the symmetric heap
 */
struct shared_heap_t* comm_local_heap();

/**
 * Check if the given PE is local to the current PE
 *
 * @param pe The other PE to check
 * @return   1 if the PE is local, otherwise 0
 */
int comm_local_has(int pe);
