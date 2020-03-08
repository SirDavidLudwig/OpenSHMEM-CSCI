#pragma once

#include <stdlib.h>

#include "heap.h"
#include "shared_mem.h"

/**
 * A wrapper for the heap
 */
struct shared_heap_t
{
	struct heap_t *heap;
	char key[];
};

/**
 * Create or open a shared heap object and map it into memory
 *
 * @param key  The name of the shared heap object
 * @param size The size of the shared heap object
 * @return     Returns a reference to the heap upon success, otherwise NULL
 */
struct shared_heap_t* shared_heap_open(char *key, size_t size);

/**
 * Open and map a shared heap object and truncate it
 *
 * @param key  The name of the shared heap object
 * @param size The size of the shared heap object
 * @return     Returns a reference to the heap upon success, otherwise NULL
 */
struct shared_heap_t* shared_heap_create(char *key, size_t size);

/**
 * Allocate space on the given shared heap
 *
 * @param shared_heap The shared heap object
 * @param bytes       The amount of memory to allocate
 * @return            The allocated memory address upon success, otherwise NULL
 */
void* shared_heap_malloc(struct shared_heap_t *shared_heap, size_t bytes);

/**
 * Free memory on the given shared heap
 *
 * @param shared_heap The shared heap object
 * @param ptr         The address of the memory block to free
 */
void shared_heap_free(struct shared_heap_t *shared_heap, void *ptr);

/**
 * Close a shared heap
 *
 * @param heap The heap to close
 */
void shared_heap_close(struct shared_heap_t *shared_heap);

/**
 * Calculate the offset from the beginning if oth heap
 *
 * @param shared_heap The shared heap object
 * @param ptr         The address of the memory block
 * @return            The heap-offset of the memory block
 */
size_t shared_heap_offset(struct shared_heap_t *shared_heap, const void *ptr);
