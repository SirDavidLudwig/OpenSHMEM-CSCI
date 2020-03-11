#include "comm_node.h"

/**
 * The shared memory region for the node
 */
static void *__region;

/**
 * The shared memory region's size
 */
static size_t __size;

/**
 * The pSync array for barriers
 */
static long *__p_sync;

/**
 * The list of nodes
 */
static host_map_t *__nodes;

/**
 * The locality map of all PEs
 */
static locality_map_t *__locality_map;

/**
 * Initialize the shared memory region for the node
 *
 * @param my_local_pe The current PE's local ID
 * @param n_local_pes The total number of PEs on this node
 * @param n_pes       The total number of PEs
 * @param n_nodes     The total number of nodes
 */
void comm_node_init(int my_local_pe, int n_local_pes, int n_pes, int n_nodes)
{
	char key[256];
	size_t p_sync_size;
	size_t nodes_size;
	size_t locality_map_size;
	void* head;

	// Calculate the size of the memory blocks in the region
	p_sync_size       = n_local_pes*sizeof(long);
	nodes_size        = n_nodes*sizeof(host_map_t);
	locality_map_size = n_pes*sizeof(locality_map_t);
	__size            = p_sync_size + nodes_size + locality_map_size;

	// Create the local symmetric heap
	shared_mem_key("node", key);
	if (my_local_pe == 0) {
		shared_mem_create(key, __size, &__region);
	} else {
		shared_mem_open(key, __size, &__region);
	}

	// Assign the pointers to their designated locations
	__p_sync       = (head = __region);
	__nodes        = (head += p_sync_size);
	__locality_map = (head += nodes_size);
}

/**
 * Finalize the node's shared memory region
 */
void comm_node_finalize()
{
	shared_mem_close(__region, __size);
}

/**
 * Map the locality of the given PEs and nodes
 *
 * @param hostname The hostname of the current node
 * @param hosts    The complete list of hosts
 * @param pes      The 2D-list of pes assocociated with `hosts`
 * @param n_hosts  The number of hosts in the list
 * @param n_pes    A list of the number of PEs for each host
 */
void comm_node_map_locality(char *hostname, char **hosts, int **pes, int n_hosts, int *n_pes)
{
	locality_map_t *map;
	char type;
	int i, j;
	int index = 0;

	for (i = 0; i < n_hosts; i++) {
		__nodes[i].addr = netutil_resolve_host(hosts[i]);
		strcpy(__nodes[i].hostname, hosts[i]);
		type = (strcmp(hostname, hosts[i]) == 0) ? PE_TYPE_LOCAL : PE_TYPE_REMOTE;
		for (j = 0; j < n_pes[i]; j++) {
			map        = &__locality_map[pes[i][j]];
			map->type  = type;
			map->index = (type == PE_TYPE_LOCAL) ? j : index++;
			map->host  = &__nodes[i];
		}
	}
}

// Accessors ---------------------------------------------------------------------------------------

/**
 * Fetch the locality map for the given PE
 *
 * @param pe The PE to fetch
 * @return   The PE's locality map
 */
locality_map_t* comm_node_map(int pe)
{
	return &__locality_map[pe];
}
