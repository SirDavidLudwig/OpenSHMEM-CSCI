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

// Error Codes -------------------------------------------------------------------------------------

#define SHM_ERROR_NONE   0    // No error has occurred
#define SHM_ERROR_CREATE 1    // Failed to create shared memory object
#define SHM_ERROR_MAP    2    // Failed to allocate memory on shared object

// Heap Management ---------------------------------------------------------------------------------

/**
 * Create and map a shared memory object. Returns -1 upon error
 */
int shm_init(int pe);

/**
 * Malloc the given size of bytes on the heap
 */
void* shm_malloc(size_t bytes);
