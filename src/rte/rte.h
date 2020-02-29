#pragma once

#include "rte_util.h"

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
 * Perform an all-gather operation
 */
void rte_all_gather(void *sendbuf, int sendsize, void *recvbuf, int recvsize);

/**
 * [Collective]
 * Perform a barrier at the RTE layer
 */
void rte_barrier();

// Accessors ---------------------------------------------------------------------------------------

/**
 * Get the hostname for the current process
 *
 * @param hostname The hostname
 * @param len      The length of the hostname
 */
void rte_hostname(char *hostname, int *len);

/**
 * Get the current process' ID
 */
int rte_my_pe();

/**
 * Get the number of processes
 */
int rte_n_pes();
