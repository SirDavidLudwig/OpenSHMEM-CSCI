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
 * The size of the symmetric heap
 */
#define HEAP_SIZE 2 << 25 // 64 megabytes

/**
 * Used to identify my shared memory objects on the shared cluster
 */
#define USER_IDENTIFIER "dwl2x"

// Error Codes -------------------------------------------------------------------------------------

#define SHM_ERROR_NONE 0    // No error has occurred
#define SHM_ERROR_OPEN 1    // Failed to create shared memory object
#define SHM_ERROR_MAP  2    // Failed to allocate memory on shared object

// Heap Management ---------------------------------------------------------------------------------

/**
 * Create and map a shared memory object. Returns -1 upon error
 */
int shared_memory_init(int pe);

/**
 * Open a shared memory object
 */
int shared_memory_open(int pe);

/**
 * Map a shared memory object into virtual memory space
 */
int shared_memory_map(int fd, void **ptr);

/**
 * Clean up any shared memory objects
 */
void shared_memory_free();

/**
 * Malloc the given size of bytes on the heap
 */
void* shared_memory_malloc(size_t bytes);

/**
 * Calculate the byte offset for the given variable relative to the start of the heap
 */
int shared_memory_offset(const void* source);
