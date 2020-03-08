#include "comm_node.h"

/**
 * Calculate the size of the memory region
 */
#define MEMORY_REGION_SIZE(n_pes) n_pes*sizeof(long)

void comm_node_init(int my_local_pe, int n_local_pes, void **region)
{
	char key[256];

	// Generate the key
	sprintf(key, "%s-shmem", SHARED_MEMORY_PREFIX);

	// Create the local symmetric heap
	if (my_local_pe == 0) {
		shared_mem_create(key, MEMORY_REGION_SIZE(n_local_pes), region);
	} else {
		shared_mem_open(key, MEMORY_REGION_SIZE(n_local_pes), region);
	}
}
