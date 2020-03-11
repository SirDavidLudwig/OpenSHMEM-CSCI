#pragma once

#include "comm_node.h"
#include "comm_local.h"
#include "comm_remote.h"

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the communication layer
 *
 * @param my_local_pe The local ID of this PE with respect to the node
 * @param n_local_pes The total number of PEs on this node
 * @param my_pe       The current PE's ID
 * @param n_pes       The total number of PEs
 * @param n_nodes     The total number of nodes
 */
void comm_init(int my_local_pe, int n_local_pes, int my_pe, int n_pes, int n_nodes);

/**
 * Finalize the communication layer
 */
void comm_finalize();

/**
 * Map the locality of the given PEs and nodes
 *
 * @param host    The hostname of hte current node
 * @param hosts   The complete list of hosts
 * @param pes     The 2D-list of pes assocociated with `hosts`
 * @param n_hosts The number of hosts in the list
 * @param n_pes   A list of the number of PEs for each host
 */
void comm_map_locality(char *host, char **hosts, int **pes, int n_hosts, int *n_pes);

/**
 * Wire up the processes
 */
void comm_wireup();

// Communication Methods ---------------------------------------------------------------------------

/**
 * Get a value from another process
 *
 * @param dest   The destination to store the result
 * @param source The remote variable to get from
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_get(void *dest, const void* source, size_t bytes, int pe);

/**
 * Send a value from another process
 *
 * @param dest   The destination to store the result
 * @param source The local variable to send
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_put(void *dest, const void *source, size_t bytes, int pe);

/**
 * [Blocking]
 * Wait for the worker thread to flush requests
 */
void comm_flush();

// Accessors ---------------------------------------------------------------------------------------

/**
 * Get the symmetric heap
 *
 * @return A reference to the symmetric heap
 */
struct shared_heap_t* comm_symmetric_heap();

/**
 * Determine if the given PE is local to the current PE
 *
 * @param  pe The other PE to check
 * @return 1 if the PE is local, otherwise 0
 */
int comm_is_local(int pe);
