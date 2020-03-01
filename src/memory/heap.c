#include "heap.h"

// Macros ------------------------------------------------------------------------------------------

/**
 * Calculate the end address of a block
 */
#define BLOCK_END(a) ((void*) (a->ptr + a->size))

/**
 * Calculate the amount of space available after a block
 */
#define BLOCK_GAP(a,b) ((size_t) (((a->next == NULL) ? BLOCK_END(b) : a->next) - BLOCK_END(a)))

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
struct heap_t* heap_init(void *region, size_t size)
{
	struct heap_t *heap;

	// Create the heap structure
	heap = malloc(sizeof(struct heap_t));
	heap->tail = NULL;
	heap->head = NULL;
	heap->ptr = region;
	heap->size = size;

	// Return the heap object
	return heap;
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
	block->next = NULL;

	// Get the head
	head = heap->head;
	if (head == NULL) {
		block->ptr = heap->ptr;
		heap->tail = block;
	} else {
		// Find a block that we can insert into
		while (BLOCK_GAP(head, heap) < bytes) {
			if (NEXT_BLOCK(head, heap) == heap->head) {
				perror("Unable to malloc onto heap");
				return NULL;
			}
		}
		block->ptr = BLOCK_END(head);
		block->next = head->next;
		head->next = block;
	}

	// Update the head
	heap->head = block;

	// Return the address pointer
	return block->ptr;
}

/**
 * @TODO
 * Free memory on a heap
 */
void heap_free(struct heap_t *heap, void *ptr)
{
	//
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
