#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi/mpi.h>

#define HOSTNAME_SIZE 256

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the runtime layer
 */
void rte_init();

/**
 * Finalize the runtime layer
 */
void rte_finalize();

// Interface ---------------------------------------------------------------------------------------

/**
 * Perform a barrier at the RTE layer
 */
void rte_barrier();

/**
 * Get the host map of all PEs
 */
char** rte_pe_hosts();

/**
 * Get the current process' ID
 */
int rte_my_pe();

/**
 * Get the number of processes
 */
int rte_n_pes();
