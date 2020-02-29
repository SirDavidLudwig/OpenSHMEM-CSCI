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
	printf("MPI RTE initialized\n");
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
 * Perform an all-gather operation
 */
void rte_all_gather(void *sendbuf, int sendsize, void *recvbuf, int recvsize)
{
	MPI_Allgather(sendbuf, sendsize, MPI_BYTE, recvbuf, recvsize, MPI_BYTE, MPI_COMM_WORLD);
}

/**
 * [Collective]
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
