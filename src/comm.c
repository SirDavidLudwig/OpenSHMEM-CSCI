#include "comm.h"

#include "memory/heap.h"
#include "memory/shared_mem.h"

#define MEM_PREFIX "dwl2x"
#define HEAP_SIZE  1024

/**
 * Initialize the communication layer
 */
void comm_init(int my_pe, int n_pes)
{
	void *region;
	char key[255];
	sprintf(key, "%s-shmem-%d", MEM_PREFIX, my_pe);

	shared_mem_create(key, HEAP_SIZE, &region);
	heap = heap_init(region, HEAP_SIZE);
	printf("Shared memory initialized\n");
}

/**
 * Finalize the communication layer
 */
void comm_finalize()
{
	//
}

/**
 * Wire up the processes
 */
void comm_wireup(char **hostmap)
{
	//
}
