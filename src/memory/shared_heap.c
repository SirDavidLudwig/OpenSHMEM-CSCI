#include "shared_heap.h"

/**
 * Create or open a shared heap object and map it into memory
 *
 * @param key  The name of the shared heap object
 * @param size The size of the shared heap object
 * @return     Returns a reference to the heap upon success, otherwise NULL
 */
struct shared_heap_t* shared_heap_open(char *key, size_t size)
{
	struct shared_heap_t *shared_heap;
	void *region;

	// Open the shared memory region
	if (-1 == shared_mem_open(key, size, &region)) {
		return NULL;
	}

	// Create an instance of the shared heap
	shared_heap = malloc(sizeof(struct shared_heap_t) + strlen(key) + 1);
	shared_heap->heap = heap_init(region, size);
	strcpy(shared_heap->key, key);

	// Map the heap to a heap and return
	return shared_heap;
}

/**
 * Open and map a shared heap object and truncate it
 *
 * @param key  The name of the shared heap object
 * @param size The size of the shared heap object
 * @return     Returns a reference to the heap upon success, otherwise NULL
 */
struct shared_heap_t* shared_heap_create(char *key, size_t size)
{
	struct shared_heap_t *shared_heap;
	void *region;

	// Open the shared memory region
	if (-1 == shared_mem_create(key, size, &region)) {
		return NULL;
	}

	// Create an instance of the shared heap
	shared_heap = malloc(sizeof(struct shared_heap_t) + strlen(key) + 1);
	shared_heap->heap = heap_init(region, size);
	strcpy(shared_heap->key, key);

	// Map the heap to a heap and return
	return shared_heap;
}

/**
 * Allocate space on the given shared heap
 *
 * @param shared_heap The shared heap object
 * @param bytes       The amount of memory to allocate
 * @return            The allocated memory address upon success, otherwise NULL
 */
void* shared_heap_malloc(struct shared_heap_t* shared_heap, size_t bytes)
{
	return heap_malloc(shared_heap->heap, bytes);
}

/**
 * Free memory on the given shared heap
 *
 * @param shared_heap The shared heap object
 * @param ptr         The address of the memory block to free
 */
void shared_heap_free(struct shared_heap_t *shared_heap, void *ptr)
{
	heap_free(shared_heap->heap, ptr);
}

/**
 * Close a shared heap
 *
 * @param shared_heap The heap to close
 */
void shared_heap_close(struct shared_heap_t *shared_heap)
{
	// Unmap the shared memory
	shared_mem_close(shared_heap->heap->ptr, shared_heap->heap->size);

	// Free the heap from memory
	heap_finalize(shared_heap->heap);
}

/**
 * Calculate the offset from the beginning if oth heap
 *
 * @param shared_heap The shared heap object
 * @param ptr         The address of the memory block
 * @return            The heap-offset of the memory block
 */
size_t shared_heap_offset(struct shared_heap_t *shared_heap, const void *ptr)
{
	return heap_offset(shared_heap->heap, ptr);
}
