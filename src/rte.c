#include "rte.h"

// Private Data Members ----------------------------------------------------------------------------

/**
 * The current process' ID
 */
static int __MY_PE;

/**
 * The number of processes
 */
static int __N_PES;

/**
 * Map all hosts by PE ID
 */
static char **__PE_HOSTS;

// Private Functions -------------------------------------------------------------------------------

static void __rte_map_pes()
{
	char hosts[__N_PES * HOSTNAME_SIZE];
	char hostname[HOSTNAME_SIZE];
	int i, len;

	// Allocate a hostname for each PE
	__PE_HOSTS = malloc(__N_PES * sizeof(char *));

	// Get the hostname of the process
	rte_hostname(hostname, &len);

	// Let everyone know who is who and where they can be found
	MPI_Allgather(hostname, HOSTNAME_SIZE, MPI_CHAR, hosts, HOSTNAME_SIZE, MPI_CHAR, MPI_COMM_WORLD);

	// Copy from 1D array to 2D array
	for (i = 0; i < __N_PES; i++) {
		__PE_HOSTS[i] = malloc(HOSTNAME_SIZE);
		strcpy(__PE_HOSTS[i], hosts + i * HOSTNAME_SIZE);
	}
}

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialization the runtime layer
 */
void rte_init()
{
	// MPI init
	MPI_Init(NULL, NULL);

	// PE information
	MPI_Comm_rank(MPI_COMM_WORLD, &__MY_PE);
	MPI_Comm_size(MPI_COMM_WORLD, &__N_PES);

	// Create the host map of the PEs
	__rte_map_pes();
}

/**
 * Finalize the runtime layer
 */
void rte_finalize()
{
	MPI_Finalize();
}

// Interface ---------------------------------------------------------------------------------------

/**
 * Perform a barrier at the RTE layer
 */
void rte_barrier()
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
 * Create a host map of all processes
 */
char** rte_pe_hosts()
{
	return __PE_HOSTS;
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
