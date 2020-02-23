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
char *__PE_HOST_MAP;

// Private Functions -------------------------------------------------------------------------------

/**
 * Create a map of all processes
 */
void __rte_create_map()
{
	char hostname[255];
	int i, len;

	// Allocate a hostname for each PE
	__PE_HOST_MAP = malloc(__N_PES * HOSTNAME_SIZE);

	// Get the hostname of the process
	MPI_Get_processor_name(hostname, &len);

	// Let everyone know who is who and where they can be found
	MPI_Allgather(hostname, HOSTNAME_SIZE, MPI_CHAR, __PE_HOST_MAP, HOSTNAME_SIZE, MPI_CHAR, MPI_COMM_WORLD);

	// @TODO Remove later
	printf("\nPE %d:\n", __MY_PE);
	for (i = 0; i < __N_PES; i++) {
		char *hostname = rte_pe_host(i);
		printf("The hostname is %s\n", hostname);
	}
	printf("\n");
}

// Getters -----------------------------------------------------------------------------------------

char* rte_pe_host(int pe)
{
	return __PE_HOST_MAP + pe*HOSTNAME_SIZE;
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

// Public Functions --------------------------------------------------------------------------------

/**
 * Initialize the runtime layer
 */
void rte_init()
{
	char hostname[255];
	int len;

	// MPI init
	MPI_Init(NULL, NULL);

	// PE information
	MPI_Comm_rank(MPI_COMM_WORLD, &__MY_PE);
	MPI_Comm_size(MPI_COMM_WORLD, &__N_PES);

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
