#include "rte_util.h"

/**
 * Create a map of all hostnames for the PEs
 */
char** rte_map_hosts()
{
	char **hostmap;
	char hosts[rte_n_pes() * HOSTNAME_LEN];
	char hostname;
	int i, len;

	// Allocate a hostname for each PE
	hostmap = malloc(rte_n_pes() * sizeof(char *));

	// Get the hostname of the process
	rte_hostname(hostname, &len);

	// Let everyone know who is who and where they can be fonud
	rte_all_gather(hostname, HOSTNAME_LEN, hosts, HOSTNAME_LEN);

	// Let everyone know who is who and where they can be found
	// MPI_Allgather(hostname, sizeof(hostname_t), MPI_CHAR, hosts, sizeof(hostname_t), MPI_CHAR, MPI_COMM_WORLD);

	// Copy from 1D array to 2D array
	for (i = 0; i < rte_n_pes(); i++)
	{
		hostmap[i] = malloc(HOSTNAME_LEN);
		strcpy(hostmap[i], hosts + i * HOSTNAME_LEN);
	}

	return hostmap;
}
