#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi/mpi.h>

#define HOSTNAME_SIZE 256

// Functions ---------------------------------------------------------------------------------------

/**
 * Pre-initialization requirements
 */
void rte_preinit();

/**
 * Initialize the runtime layer
 */
void rte_init();

/**
 * Finalize the runtime layer
 */
void rte_finalize();

// Getters -----------------------------------------------------------------------------------------

/**
 * Get the hostname for the given process ID
 */
char *rte_pe_host(int pe);

/**
 * Get the current process' ID
 */
int rte_my_pe();

/**
 * Get the number of processes
 */
int rte_n_pes();
