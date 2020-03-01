#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * A block of memory
 */
struct block_t
{
	void *ptr;   // The address of the first byte
	size_t size; // The size of the block in bytes
	void *next;  // The next block address
};

/**
 * A heap of memory
 */
struct heap_t
{
	void *ptr;            // The base address of the heap
	struct block_t *head; // The starting memory block
	struct block_t *tail; // The most recently allocated block
	size_t size;          // The size of the block
};

/**
 * Create a heap for the given memory region
 *
 * @param region  The starting address for the memory region
 * @param size The size of the memory region in bytes
 */
struct heap_t* heap_init(void *region, size_t size);

/**
 * Allocate memory on a heap using `next-fit`
 *
 * @param heap  The heap reference
 * @param bytes The amount of memory to allocate in bytes
 */
void* heap_malloc(struct heap_t *heap, size_t bytes);

/**
 * @todo
 * Free memory on a heap
 */
void heap_free(struct heap_t *heap, void *ptr);

/**
 * Calculate the heap-offset of a variable
 *
 * @param heap The heap reference
 * @param ptr  The address within the heap
 */
long heap_offset(struct heap_t *heap, void *ptr);
