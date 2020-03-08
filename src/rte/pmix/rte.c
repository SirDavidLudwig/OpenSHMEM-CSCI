#include "../rte.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pmix.h>

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
 * The active proc
 */
static pmix_proc_t __proc;

// Layer Management --------------------------------------------------------------------------------

/**
 * [Collective]
 * Initialization the runtime layer
 */
void rte_init()
{
	pmix_value_t *val;
	pmix_status_t rc;

	// Initialize PMIx
	if (PMIX_SUCCESS != (rc = PMIx_Init(&__proc, NULL, 0))) {
		perror("PMIx init failed");
	}

	// Save the current process' rank
	__MY_PE = __proc.rank;

	// Get the number of processes in the job
	__proc.rank = PMIX_RANK_WILDCARD;
	if (PMIX_SUCCESS != (rc = PMIx_Get(&__proc, PMIX_JOB_SIZE, NULL, 0, &val))) {
		perror("PMIx Get failed");
	}
	__N_PES = val->data.uint32;
}

/**
 * [Collective]
 * Finalize the runtime layer
 */
void rte_finalize()
{
	pmix_status_t rc;

	if (PMIX_SUCCESS != (rc = PMIx_Finalize(NULL, 0))) {
		perror("PMIx finalize failed");
	}
}

// Interface ---------------------------------------------------------------------------------------

/**
 * [Collective]
 * Get a list of all hostnames mapped by PE
 */
char** rte_hosts()
{
	char **hostmap;
	// char hosts[rte_n_pes() * HOSTNAME_LEN];
	// char hostname[HOSTNAME_LEN];
	// int i, len;

	// // Allocate a hostname for each PE
	// hostmap = malloc(rte_n_pes() * sizeof(char *));

	// // Get the hostname of the process
	// rte_hostname(hostname, &len);

	// Let everyone know who is who and where they can be found
	// MPI_Allgather(hostname, HOSTNAME_LEN, MPI_CHAR, hosts, HOSTNAME_LEN, MPI_CHAR, MPI_COMM_WORLD);

	// // Copy from 1D array to 2D array
	// for (i = 0; i < rte_n_pes(); i++) {
	// 	hostmap[i] = malloc(HOSTNAME_LEN);
	// 	strcpy(hostmap[i], hosts + i * HOSTNAME_LEN);
	// }

	return hostmap;
}

/**
 * [Collective]
 * Perform a barrier at the RTE layer
 */
void rte_barrier_all()
{
	// Build a list of procs for all PEs within the namespace
	pmix_proc_t procs[__N_PES];
	for (int i = 0; i < __N_PES; i++) {
		memcpy(procs[i].nspace, __proc.nspace, 256);
		procs[i].rank = i;
	}

	// Perform a fence/barrier
	PMIx_Fence(procs, __N_PES, NULL, 0);
}

/**
 * Get the hostname for the current process
 *
 * @param hostname The hostname
 * @param len      The length of the hostname
 */
void rte_hostname(char *hostname, int *len)
{
	gethostname(hostname, HOSTNAME_LEN);
	*len = strlen(hostname);
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
