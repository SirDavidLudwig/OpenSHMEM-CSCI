#pragma once

#include <stdlib.h>

#include "heap.h"
#include "shared_mem.h"

/**
 * Create or open a shared heap object and map it into memory
 *
 * @param key    The name of the shared heap object
 * @param size   The size of the shared heap object
 * @return       Returns the file descriptor upno success, otherwise -1
 */
struct heap_t* shared_heap_open(char *key, size_t size);

/**
 * Open and map a shared heap object and truncate it
 *
 * @param key    The name of the shared heap object
 * @param size   The size of the shared heap object
 * @return       Returns the file descriptor upno success, otherwise -1
 */
struct heap_t* shared_heap_create(char *key, size_t size);

/**
 * Close a shared heap
 *
 * @param heap The heap to close
 */
void shared_heap_close(struct heap_t *heap);
