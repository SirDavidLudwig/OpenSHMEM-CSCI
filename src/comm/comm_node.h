#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "../memory/shared_mem.h"

/**
 * Locality of a PE
 */
#define PE_TYPE_LOCAL  0
#define PE_TYPE_REMOTE 1

/**
 * The size of a locality map object
 */
#define LOCALITY_MAP_SIZE sizeof(locality_map_t)

/**
 * The possible types for a locality map's value
 */
typedef union {
	char *hostname;
	int local_id;
} locality_value_t;

/**
 * Map a PE to a local ID or a remote host
 */
typedef struct locality_map
{
	char type;
	locality_value_t value;
} locality_map_t;

/**
 * Initialize the shared memory region for the node
 *
 * @param my_local_pe The current PE's local ID
 * @param n_local_pes The total number of PEs on this node
 * @param n_pes       The total number of PEs
 * @param n_nodes     The total number of nodes
 */
void comm_node_init(int my_local_pe, int n_local_pes, int n_pes, int n_nodes);

/**
 * Finalize the node's shared memory region
 */
void comm_node_finalize();

/**
 * Map the locality of the given PEs and nodes
 *
 * @param host    The hostname of hte current node
 * @param hosts   The complete list of hosts
 * @param pes     The 2D-list of pes assocociated with `hosts`
 * @param n_hosts The number of hosts in the list
 * @param n_pes   A list of the number of PEs for each host
 */
void comm_node_map_locality(char *host, char **hosts, int **pes, int n_hosts, int *n_pes);

// Accessors ---------------------------------------------------------------------------------------

/**
 * Fetch the locality map for the given PE
 *
 * @param pe The PE to fetch
 * @return   The PE's locality map
 */
locality_map_t* comm_node_map(int pe);
