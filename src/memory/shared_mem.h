#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../common.h"

/**
 * Create a generic for shared memory key generation
 */
#define shared_mem_key(identifier, result) _Generic((identifier), \
		int: shared_mem_key_int, \
		char*: shared_mem_key_str \
	)(identifier, result)

// Heap Management ---------------------------------------------------------------------------------

/**
 * Generate the key to a shared memory object
 *
 * @param identifier The identifier for the key
 * @param key        The resulting key
 */
void shared_mem_key_int(int identifier, char key[256]);
void shared_mem_key_str(char *identifier, char key[256]);

/**
 * Create or open a shared memory object and map it into memory
 *
 * @param key    The name of the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns the file descriptor upon success, otherwise -1
 */
int shared_mem_open(char *key, size_t size, void **region);

/**
 * Open and map a shared memory object and truncate it
 *
 * @param key    The name of the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns the file descriptor upon success, otherwise -1
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
 * Unmap the given shared memory region
 *
 * @param region The shared memory region to unmap
 * @param size   The size of the shared memory object
 */
void shared_mem_close(void *region, size_t size);
