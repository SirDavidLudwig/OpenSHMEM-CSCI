#pragma once

#include <string.h>

#include "../common.h"

// Layer Management --------------------------------------------------------------------------------

/**
 * [Collective]
 * Initialize the runtime layer
 */
void rte_init();

/**
 * [Collective]
 * Finalize the runtime layer
 */
void rte_finalize();

// Collective Operations----------------------------------------------------------------------------

/**
 * [Collective]
 * Perform a barrier at the RTE layer on the current node only
 */
void rte_barrier_node();

/**
 * [Collective]
 * Perform a barrier at the RTE layer
 */
void rte_barrier_all();

// Accessors ---------------------------------------------------------------------------------------

/**
 * Get the hostname for the current process
 *
 * @param hostname The hostname
 * @param [len]    The length of the hostname
 */
void rte_hostname(char *hostname, int *len);

/**
 * Get the list of hosts
 */
void rte_hosts(char ***hosts, int *len);

/**
 * Get the current process' ID
 */
int rte_my_pe();

/**
 * Get the number of processes
 */
int rte_n_pes();

/**
 * Get the current process' local ID
 */
int rte_my_local_pe();

/**
 * Get the number of local PEs on the node
 */
int rte_n_local_pes();

/**
 * Get the total number of nodes
 */
int rte_n_nodes();

/**
 * Get a list of all PEs on a node
 */
void rte_local_peers(char *hostname, int **pes, int *len);

/**
 * Get all remote peers
 */
void rte_remote_peers(char ***hosts, int ***pes, int *n_hosts, int **n_pes);

/**
 * Get the root process for a node
 */
int rte_local_root(char *hostname);
