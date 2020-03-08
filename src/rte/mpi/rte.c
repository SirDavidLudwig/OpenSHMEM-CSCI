#include "../rte.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi/mpi.h>

// Private Data Members ----------------------------------------------------------------------------

/**
 * The current process' ID
 */
static int __MY_PE;

/**
 * The number of processes
 */
static int __N_PES;

// Layer Management --------------------------------------------------------------------------------

/**
 * [Collective]
 * Initialization the runtime layer
 */
void rte_init()
{
	// MPI init
	MPI_Init(NULL, NULL);

	// PE information
	MPI_Comm_rank(MPI_COMM_WORLD, &__MY_PE);
	MPI_Comm_size(MPI_COMM_WORLD, &__N_PES);
}

/**
 * [Collective]
 * Finalize the runtime layer
 */
void rte_finalize()
{
	MPI_Finalize();
}

// Interface ---------------------------------------------------------------------------------------

/**
 * [Collective]
 * Get a list of all hostnames mapped by PE
 */
char** rte_hosts()
{
	char **hostmap;
	char hosts[rte_n_pes() * HOSTNAME_LEN];
	char hostname[HOSTNAME_LEN];
	int i, len;

	// Allocate a hostname for each PE
	hostmap = malloc(rte_n_pes() * sizeof(char *));

	// Get the hostname of the process
	rte_hostname(hostname, &len);

	// Let everyone know who is who and where they can be found
	MPI_Allgather(hostname, HOSTNAME_LEN, MPI_CHAR, hosts, HOSTNAME_LEN, MPI_CHAR, MPI_COMM_WORLD);

	// Copy from 1D array to 2D array
	for (i = 0; i < rte_n_pes(); i++) {
		hostmap[i] = malloc(HOSTNAME_LEN);
		strcpy(hostmap[i], hosts + i * HOSTNAME_LEN);
	}

	return hostmap;
}

/**
 * [Collective]
 * Perform a barrier at the RTE layer
 */
void rte_barrier_all()
{
	MPI_Barrier(MPI_COMM_WORLD);
}

/**
 * Get the hostname for the current process
 *
 * @param hostname The hostname
 * @param len      The length of the hostname
 */
void rte_hostname(char *hostname, int *len)
{
	MPI_Get_processor_name(hostname, len);
}

/**
 * Get the current process' ID
 */
int rte_my_pe()
{
	return __MY_PE;
}

/**
 * Get the number of processes
 */
int rte_n_pes()
{
	return __N_PES;
}
