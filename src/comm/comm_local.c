#include "comm_local.h"

/**
 * The symmetric heap
 */
static struct shared_heap_t *__symmetric_heap;

/**
 * A list of shared memory mappings for each PE
 */
static void **__shared_memory;

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the local communication layer
 *
 * @param my_pe The current process' ID
 */
void comm_local_init(int my_pe)
{
	char key[255];

	// Generate the key
	sprintf(key, "%s-shmem-%d", SHARED_MEMORY_PREFIX, my_pe);

	// Create the local symmetric heap
	__symmetric_heap = shared_heap_create(key, SYMMETRIC_HEAP_SIZE);
}

/**
 * Finalize the local communication layer
 *
 * @param n_pes The number of processes
 */
void comm_local_finalize(int n_pes)
{
	// Close all shared memory heaps
	for (int i = 0; i < n_pes; i++) {
		if (__shared_memory[i] != NULL) {
			shared_heap_close(__shared_memory[i]);
		}
	}

	// Close the symmetric heap
	shared_heap_close(__symmetric_heap);

	// Free the memory region
	free(__shared_memory);
}

/**
 * Wire up local processes
 *
 * @param my_pe   The current process' ID
 * @param n_pes   The number of processes
 * @param hostmap A map of hosts indexed by PE ID
 */
void comm_local_wireup(int my_pe, int n_pes, char **hostmap)
{
	char key[255];

	// Allocate the memory to store each symmetric heap
	__shared_memory = malloc(n_pes*sizeof(void*));
	memset(__shared_memory, 0, n_pes*sizeof(void*));

	// Map all local symmetric heaps into memory
	for (int i = 0; i < n_pes; i++) {
		if (strcmp(hostmap[i], hostmap[my_pe]) == 0 && i != my_pe) {
			sprintf(key, "%s-shmem-%d", SHARED_MEMORY_PREFIX, i);
			if (!shared_mem_open(key, SYMMETRIC_HEAP_SIZE, &__shared_memory[i])) {
				perror("Failed to open shared memory");
				return;
			}
		}
	}
}

// Communication Methods ---------------------------------------------------------------------------

/**
 * Get a value from a local PE
 *
 * @param pe   The PE to communicate with
 * @param dest The destination variable to put the result
 * @param src  The source offset position within the heap
 * @param size The number of bytes to send
 */
void comm_local_get(int pe, void *dest, long src, size_t size)
{
	memcpy(dest, __shared_memory[pe] + src, size);
}

/**
 * Put a value into a local PE
 *
 * @param pe   The PE to communicate with
 * @param dest The destination offset position within the heap
 * @param src  The source variable to send
 * @param size The number of bytes to send
 */
void comm_local_put(int pe, long dest, const void *src, size_t size)
{
	memcpy(__shared_memory[pe] + dest, src, size);
}

// Accessors ---------------------------------------------------------------------------------------

/**
 * Get a reference to the symmetric heap
 *
 * @return A reference to the symmetric heap
 */
struct shared_heap_t* comm_local_heap()
{
	return __symmetric_heap;
}

/**
 * Check if the given PE is local to the current PE
 *
 * @param pe The other PE to check
 * @return   1 if the PE is local, otherwise 0
 */
int comm_local_has(int pe)
{
	return __shared_memory[pe] != NULL;
}
