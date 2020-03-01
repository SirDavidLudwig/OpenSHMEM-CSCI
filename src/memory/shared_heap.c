#include "shared_heap.h"

/**
 * Create or open a shared heap object and map it into memory
 *
 * @param key    The name of the shared heap object
 * @param size   The size of the shared heap object
 * @return       Returns the file descriptor upno success, otherwise -1
 */
struct heap_t* shared_heap_open(char *key, size_t size)
{
	void *region;

	// Open the shared memory region
	if (-1 == shared_mem_open(key, size, &region)) {
		return NULL;
	}

	// Map the heap to a heap and return
	return heap_init(region, size);
}

/**
 * Open and map a shared heap object and truncate it
 *
 * @param key    The name of the shared heap object
 * @param size   The size of the shared heap object
 * @return       Returns the file descriptor upno success, otherwise -1
 */
struct heap_t* shared_heap_create(char *key, size_t size)
{
	void *region;

	// Open the shared memory region
	if (-1 == shared_mem_create(key, size, &region)) {
		return NULL;
	}

	// Map the heap to a heap and return
	return heap_init(region, size);
}

/**
 * Close a shared heap
 *
 * @param heap The heap to close
 */
void shared_heap_close(struct heap_t *heap)
{
	// Unmap the shared memory
	shared_mem_unmap(heap->ptr, heap->size);

	// Free the heap from memory
	heap_finalize(heap);
}
