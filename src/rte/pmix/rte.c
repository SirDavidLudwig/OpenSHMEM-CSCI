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
 * @param [len]    The length of the hostname
 */
void rte_hostname(char *hostname, int *len)
{
	gethostname(hostname, HOSTNAME_LEN);
	if (len != NULL) {
		*len = strlen(hostname);
	}
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

/**
 * Get the current process' local ID
 */
int rte_my_local_pe()
{
	pmix_status_t rc;
	pmix_value_t *val;
	int result;

	// Prepare the proc
	__proc.rank = __MY_PE;

	// Get the process' local ID
	if (PMIX_SUCCESS != (rc = PMIx_Get(&__proc, PMIX_LOCAL_RANK, NULL, 0, &val))) {
		perror("PMIx Get failed to retrieve local rank");
		return -1;
	}

	// free the value and return the result
	result = (int) val->data.uint16;
	PMIX_VALUE_RELEASE(val);
	return result;
}

/**
 * Get the number of local PEs on the node
 */
int rte_n_local_pes()
{
	pmix_status_t rc;
	pmix_value_t *val;
	int result;

	// Prepare the proc
	__proc.rank = PMIX_RANK_WILDCARD;

	// Get the number of PEs on this node
	if (PMIX_SUCCESS != (rc = PMIx_Get(&__proc, PMIX_LOCAL_SIZE, NULL, 0, &val))) {
		perror("PMIx Get failed to retrieve local job size");
		return -1;
	}

	// Free the value and return the result
	result = (int) val->data.uint32;
	PMIX_VALUE_RELEASE(val);
	return result;
}

/**
 * Get a list of all local PEs
 */
void rte_local_peers(char *hostname, int **pes, int *len)
{
	pmix_status_t rc;
	pmix_proc_t *procs;
	size_t n_procs;
	int i;

	// Get the list of local peers
	if (PMIX_SUCCESS != (rc = PMIx_Resolve_peers(hostname, __proc.nspace, &procs, &n_procs))) {
		perror("PMIx failed to resolve local peers");
		return;
	}

	// Allocate the result array and copy the values
	*pes = malloc(n_procs*sizeof(int));
	*len = (int) n_procs;
	for (i = 0; i < n_procs; i++) {
		(*pes)[i] = (int) procs[i].rank;
	}
}

/**
 * Get the root process for a node
 */
int rte_local_root(char *hostname)
{
	int *peers;
	int n_peers;

	// fetch the list of local peers
	rte_local_peers(hostname, &peers, &n_peers);

	// Return the first item
	return peers[0];
}
