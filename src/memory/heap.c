#include "heap.h"

#define NUM_BUCKETS 1023

// Macros ------------------------------------------------------------------------------------------

/**
 * Calculate the end address of a block
 */
#define BLOCK_END(a) ((void*) (a->ptr + a->size))

/**
 * Calculate the amount of space available after a block
 */
#define BLOCK_GAP(a,b) ((size_t) (((a->next == NULL) ? BLOCK_END(b) : a->next->ptr) - BLOCK_END(a)))

/**
 * Calculate the next block for `next-fit`
 */
#define NEXT_BLOCK(a,b) (a = (a->next == NULL) ? b->tail : a->next)

// Interface ---------------------------------------------------------------------------------------

/**
 * Create a heap for the given memory region
 *
 * @param region  The starting address for the memory region
 * @param size The size of the memory region in bytes
 */
struct heap_t *heap_init(void *region, size_t size)
{
	struct heap_t *heap;

	// Create the heap structure
	heap = malloc(sizeof(struct heap_t));
	heap->block_map = hashmap_init(NUM_BUCKETS);
	heap->ptr = region;
	heap->size = size;

	// Create a dummy node
	heap->tail = heap->head = malloc(sizeof(struct block_t));
	heap->tail->ptr = heap->ptr;
	heap->tail->size = 0;

	// Return the heap object
	return heap;
}

/**
 * Destroy and free a heap structure
 *
 * @param heap The heap to free
 */
void heap_finalize(struct heap_t *heap)
{
	struct block_t *block, *next;

	// Free all memory blocks
	for (block = heap->head; block != NULL; block = next) {
		next = block->next;
		free(block);
	}

	// Free the heap object
	free(heap);
}

/**
 * Allocate memory on a heap using `next-fit`
 *
 * @param heap  The heap reference
 * @param bytes The amount of memory to allocate in bytes
 */
void* heap_malloc(struct heap_t *heap, size_t bytes)
{
	struct block_t *block, *start, *head;

	// Allocate the block of memory
	block = malloc(sizeof(struct block_t));
	block->size = bytes;

	// Find a block that we can insert into
	for (head = heap->head; BLOCK_GAP(head, heap) < bytes;) {
		if (NEXT_BLOCK(head, heap) == heap->head) {
			return NULL;
		}
	}

	// Update the next block's hashmap info
	if (head->next) {
		hashmap_set(heap->block_map, head->next, block);
	}

	// Insert the block into memory and the hashmap
	block->ptr = BLOCK_END(head);
	block->next = head->next;
	head->next = block;
	hashmap_set(heap->block_map, block->ptr, head);

	// Update the head
	heap->head = block;

	// Return the address pointer
	return block->ptr;
}

/**
 * Free memory on a heap
 *
 * @param heap The heap reference
 * @param ptr  The address within the heap
 */
void heap_free(struct heap_t *heap, void *ptr)
{
	struct block_t *block, *freed;

	// Find the memory block, exit if not found
	if (NULL == (block = hashmap_get(heap->block_map, ptr))) {
		return;
	}

	// Remove the block
	freed = block->next;
	block->next = freed->next;

	free(freed);
}

/**
 * Calculate the heap-offset of a variable
 *
 * @param heap The heap reference
 * @param ptr  The address within the heap
 */
long heap_offset(struct heap_t *heap, void *ptr)
{
	return ptr - heap->ptr;
}
