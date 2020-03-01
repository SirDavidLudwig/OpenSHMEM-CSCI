#include "comm_local.h"

/**
 * Initialize the local communication layer
 *
 * @param my_pe The current process' ID
 * @return      The shared heap object
 */
struct shared_heap_t* comm_local_init(int my_pe)
{
	char key[255];

	// Generate the key
	sprintf(key, "%s-shmem-%d", SHARED_MEMORY_PREFIX, my_pe);

	// Create the local symmetric heap
	return shared_heap_create(key, SYMMETRIC_HEAP_SIZE);
}

/**
 * Finalize the local communication layer
 *
 * @param n_pes         The number of processes
 * @param heap          A reference to the symmetric heap
 * @param shared_memory A list of shared memory address pointers
 */
void comm_local_finalize(int n_pes, struct shared_heap_t *heap, void **shared_memory)
{
	// Close all shared memory heaps
	for (int i = 0; i < n_pes; i++) {
		if (shared_memory[i] != NULL) {
			shared_heap_close(shared_memory[i]);
		}
	}
	// Free the memory region
	free(shared_memory);
}

/**
 * Wire up local processes
 *
 * @param my_pe   The current process' ID
 * @param n_pes   The number of processes
 * @param hostmap A map of hosts indexed by PE ID
 * @return        A list of shared memory pointers
 */
void** comm_local_wireup(int my_pe, int n_pes, char **hostmap)
{
	char key[255];
	void **shared_memory;

	// Allocate the memory to store each symmetric heap
	shared_memory = malloc(n_pes*sizeof(void*));
	memset(shared_memory, 0, n_pes*sizeof(void*));

	// Map all local symmetric heaps into memory
	for (int i = 0; i < n_pes; i++) {
		if (strcmp(hostmap[i], hostmap[my_pe]) == 0 && i != my_pe) {
			sprintf(key, "%s-shmem-%d", SHARED_MEMORY_PREFIX, i);
			if (!shared_mem_open(key, SYMMETRIC_HEAP_SIZE, &shared_memory[i])) {
				perror("Failed to open shared memory");
				return NULL;
			}
		}
	}

	return shared_memory;
}

// Communication Methods ---------------------------------------------------------------------------

/**
 * Get a value from a local PE
 *
 * @param heap The heap to get from
 * @param dest The destination variable to put the result
 * @param src  The source offset position within the heap
 * @param size The number of bytes to send
 */
void comm_local_get(void *heap, void *dest, long src, size_t size)
{
	memcpy(dest, heap + src, size);
}

/**
 * Put a value into a local PE
 *
 * @param heap The heap to put to
 * @param dest The destination offset position within the heap
 * @param src  The source variable to send
 * @param size The number of bytes to send
 */
void comm_local_put(void *heap, long dest, const void *src, size_t size)
{
	memcpy(heap + dest, src, size);
}
