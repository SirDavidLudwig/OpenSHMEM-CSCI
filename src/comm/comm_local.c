#include "comm_local.h"

/**
 * The current process' local PE
 */
static int __my_local_pe;

/**
 * The number of local PEs on the node
 */
static int __n_local_pes;

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
 * @param my_local_pe The current process' local ID
 * @param n_local_pes The total number of local PEs on the node
 */
void comm_local_init(int my_local_pe, int n_local_pes)
{
	char key[256];

	__my_local_pe = my_local_pe;
	__n_local_pes = n_local_pes;

	// Generate the shared memory key
	shared_mem_key(my_local_pe, key);

	// Create the local symmetric heap
	__symmetric_heap = shared_heap_create(key, SYMMETRIC_HEAP_SIZE);
}

/**
 * Finalize the local communication layer
 */
void comm_local_finalize()
{
	// Close all shared memory heaps
	for (int i = 0; i < __n_local_pes; i++) {
		if (__shared_memory[i] != NULL) {
			shared_mem_close(__shared_memory[i], SYMMETRIC_HEAP_SIZE);
		}
	}

	// Close the symmetric heap
	shared_heap_close(__symmetric_heap);

	// Free the memory region
	free(__shared_memory);
}

/**
 * Wire up local processes
 */
void comm_local_wireup()
{
	char key[256];

	// Allocate the memory to store each symmetric heap
	__shared_memory = malloc(__n_local_pes*sizeof(void*));

	// Map all local symmetric heaps into memory
	for (int i = 0; i < __n_local_pes; i++) {
		if (i != __my_local_pe) {
			shared_mem_key(i, key);
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

/**
 * Calculate the offset of the given pointer on the heap
 */
size_t comm_local_offset(const void* ptr)
{
	return shared_heap_offset(__symmetric_heap, ptr);
}
