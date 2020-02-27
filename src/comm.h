#pragma once

#include "network.h"
#include "rte.h"
#include "shared_memory.h"
#include "worker.h"

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the communication layer
 *
 * @param pe          The current PE's ID
 * @param n_pes       The number of PEs
 * @param pe_host_map A map of all PE's hostnames by PE ID
 */
void comm_init(int pe, int n_pes, char **pe_host_map);

/**
 * Wire up the PE communication layer
 */
void comm_wireup();

/**
 * Finalize the communication layer
 */
void comm_finalize();

// Interface ---------------------------------------------------------------------------------------

/**
 * Determine if the given PE is local to the current PE
 *
 * @param pe The other PE to check
 */
int comm_is_local(int pe);

/**
 * Determine if a PE is local to another PE
 *
 * @param pe1 The first PE's ID
 * @param pe2 The other PE's ID
 */
int comm_are_local(int pe1, int pe2);

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
