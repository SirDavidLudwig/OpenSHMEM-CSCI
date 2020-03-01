#pragma once

#include <string.h>

/**
 * The maximum hostname length
 */
#define HOSTNAME_LEN 256

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
 * Perform a barrier at the RTE layer
 */
void rte_barrier();

/**
 * [Collective]
 * Get a list of all hostnames mapped by PE
 */
char** rte_hosts();

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
