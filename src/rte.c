#include "rte.h"

// Private Data Members ----------------------------------------------------------------------------

/**
 * The current process' ID
 */
int __MY_PE;

/**
 * The number of processes
 */
int __N_PES;

/**
 * The map of the processes
 */
char **__PE_HOST_MAP;

// Private Functions -------------------------------------------------------------------------------

/**
 * Create a map of all processes
 */
void __rte_create_map()
{
	char hosts[__N_PES * HOSTNAME_SIZE];
	char hostname[HOSTNAME_SIZE];
	int i, len;

	// Allocate a hostname for each PE
	__PE_HOST_MAP = malloc(__N_PES * HOSTNAME_SIZE);

	// Get the hostname of the process
	MPI_Get_processor_name(hostname, &len);

	// Let everyone know who is who and where they can be found
	MPI_Allgather(hostname, HOSTNAME_SIZE, MPI_CHAR, hosts, HOSTNAME_SIZE, MPI_CHAR, MPI_COMM_WORLD);

	// Copy from 1D array to 2D array
	for (i = 0; i < __N_PES; i++) {
		__PE_HOST_MAP[i] = malloc(HOSTNAME_SIZE);
		strcpy(__PE_HOST_MAP[i], hosts + i*HOSTNAME_SIZE);
	}
}

// Layer Management --------------------------------------------------------------------------------

/**
 * Pre-initialization requirements
 */
void rte_preinit()
{
	// MPI init
	MPI_Init(NULL, NULL);

	// PE information
	MPI_Comm_rank(MPI_COMM_WORLD, &__MY_PE);
	MPI_Comm_size(MPI_COMM_WORLD, &__N_PES);
}

/**
 * Initialize the runtime layer
 */
void rte_init()
{
	// Create the node map
	__rte_create_map();
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
 * Get the host map of all PEs
 */
char **rte_pe_hosts()
{
	return __PE_HOST_MAP;
}

/**
 * Get the hostname for the given process ID
 */
char *rte_pe_host(int pe)
{
	return __PE_HOST_MAP[pe];
}

/**
 * Determine if a process is local to this process
 */
int rte_is_local_to(int pe)
{
	return strcmp(rte_pe_host(__MY_PE), rte_pe_host(pe)) == 0;
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
