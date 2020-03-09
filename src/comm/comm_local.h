#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../memory/shared_heap.h"

/**
 * The size of the symmetric heap
 */
#define SYMMETRIC_HEAP_SIZE 2 << 25

// Interface ---------------------------------------------------------------------------------------

/**
 * Initialize the local communication layer
 *
 * @param my_local_pe The current process' local ID
 * @param n_local_pes The total number of local PEs on the node
 */
void comm_local_init(int my_local_pe, int n_local_pes);

/**
 * Finalize the local communication layer
 */
void comm_local_finalize();

/**
 * Wire up local processes
 */
void comm_local_wireup();

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

/**
 * Calculate the offset of the given pointer on the heap
 */
size_t comm_local_offset(const void* ptr);
