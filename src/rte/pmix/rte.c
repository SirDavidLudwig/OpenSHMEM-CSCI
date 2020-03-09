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
 * Perform a barrier at the RTE layer on the current node only
 */
void rte_barrier_node()
{
	pmix_proc_t procs[rte_n_local_pes()];
	char hostname[HOSTNAME_LEN];
	int *pes;
	int n_pes;
	int i;

	// Get the list of local peers
	rte_local_peers(hostname, &pes, &n_pes);

	// Copy the PE info into the procs list
	for (i = 0; i < n_pes; i++) {
		memcpy(procs[i].nspace, __proc.nspace, 256);
		procs[i].rank = pes[i];
	}

	// Perform a fence/barrier
	PMIx_Fence(procs, n_pes, NULL, 0);

	// Free memory
	free(pes);
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
 * Get the list of hosts
 */
void rte_hosts(char ***hosts, int *len)
{
	pmix_status_t rc;
	pmix_value_t *val;
	char *hoststr;
	char *host;
	*len = 0;

	// Get the list of hosts
	if (PMIX_SUCCESS != (rc = PMIx_Resolve_nodes(__proc.nspace, &hoststr))) {
		perror("Fail to resolve hosts");
		return;
	};

	// Allocate memory for the list
	*hosts = malloc(rte_n_nodes()*sizeof(char*));

	// Copy the hosts from the string list into the array
	for (host = strtok(hoststr, ","); host != NULL; host = strtok(NULL, ","), *len += 1) {
		(*hosts)[*len] = malloc(HOSTNAME_LEN*sizeof(char));
		strcpy((*hosts)[*len], host);
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
 * Get the total number of nodes
 */
int rte_n_nodes()
{
	pmix_status_t rc;
	pmix_value_t *val;
	int result;

	// Prepare the proc
	__proc.rank = PMIX_RANK_WILDCARD;

	// Get the number of PEs on this node
	if (PMIX_SUCCESS != (rc = PMIx_Get(&__proc, PMIX_NUM_NODES, NULL, 0, &val))) {
		perror("PMIx Get failed to retrieve the number of nodes");
		return -1;
	}

	// Free the value and return the result
	result = (int) val->data.uint32;
	PMIX_VALUE_RELEASE(val);
	return result;
}

/**
 * Get a list of all PEs on a node
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
	*len = (int) n_procs;
	*pes = malloc(n_procs*sizeof(int));
	for (i = 0; i < n_procs; i++) {
		(*pes)[i] = (int) procs[i].rank;
	}
}

/**
 * Get the list of nodes and PEs
 */
void rte_remote_peers(char ***hosts, int ***pes, int *n_hosts, int **n_pes)
{
	int i;

	// Get the list of nodes
	rte_hosts(hosts, n_hosts);

	// Allocate the memory for the PE list (ignoring our host)
	*pes = malloc((*n_hosts)*sizeof(int*));
	*n_pes = malloc((*n_hosts)*sizeof(int));

	// Resolve the PEs for each node
	for (i = 0; i < *n_hosts; i++) {
		rte_local_peers((*hosts)[i], &((*pes)[i]), &(*n_pes)[i]);
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
