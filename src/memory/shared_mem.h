#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

// Constants ---------------------------------------------------------------------------------------

/**
 * Used to identify my shared memory objects on the shared cluster
 */
#define USER_IDENTIFIER "dwl2x"

// Heap Management ---------------------------------------------------------------------------------

/**
 * Create or open a shared memory object and map it into memory
 *
 * @param key    The name of the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns the file descriptor upno success, otherwise -1
 */
int shared_mem_open(char *key, size_t size, void **region);

/**
 * Open and map a shared memory object and truncate it
 *
 * @param key    The name of the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns the file descriptor upno success, otherwise -1
 */
int shared_mem_create(char *key, size_t size, void **region);

/**
 * Map a shared memory object into virtual memory space
 *
 * @param fd     The file descriptor for the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns 0 for success, -1 for failure
 */
int shared_mem_map(int fd, size_t size, void **region);

/**
 * Clean up any shared memory objects
 *
 * @param key    The name of the shared memory object
 * @param region The shared memory region to unmap
 * @param size   The size of the shared memory object
 */
void shared_mem_unmap(char *key, void *region, size_t size);
