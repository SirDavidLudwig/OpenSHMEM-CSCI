#pragma once

#include "network.h"
#include "rte.h"
#include "shared_memory.h"
#include "worker.h"

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the communication layer
 */
void comm_init();

/**
 * Finalize the communication layer
 */
void comm_finalize();

// Interface ---------------------------------------------------------------------------------------

/**
 * Get a value from another process
 *
 * @param dest   A reference to where to store the result
 * @param source The offset position relative to the beginning of the symmetric heap
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_get(void *dest, const int source, size_t bytes, int my_pe, int dest_pe);

/**
 * Send a value from another process
 *
 * @param dest   The offset position relative to the beginning of the symmetric heap
 * @param source A reference to the item to send
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_put(int dest, const void *source, size_t bytes, int my_pe, int dest_pe);

/**
 * [Blocking]
 * Wait for the worker thread to flush requests
 */
void comm_flush();
