#pragma once

#include "../memory/shared_heap.h"

/**
 * Initialize the communication layer
 */
void comm_init(int my_pe, int n_pes);

/**
 * Finalize the communication layer
 */
void comm_finalize();

/**
 * Wire up the processes
 */
void comm_wireup(char **hostmap);

// Accessors ---------------------------------------------------------------------------------------

/**
 * Get the symmetric heap
 *
 * @return A reference to the symmetric heap
 */
struct heap_t* comm_symmetric_heap();
