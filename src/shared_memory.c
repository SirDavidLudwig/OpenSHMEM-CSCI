#include "shared_memory.h"

// Private Members Data ----------------------------------------------------------------------------

/**
 * The symmetric heap
 */
void* __sym_heap = 0;

/**
 * A pointer to the end of the symmetric heap
 */
int __heap_ptr;

// Heap Management ---------------------------------------------------------------------------------

/**
 * Create and map a shared memory object. Returns -1 upon error
 */
int shared_memory_init(int pe)
{
	int fd;
	int result;

	// Create and allocate the shared memory object
	if (-1 == (fd = shared_memory_open(pe))) {
		perror("Shared memory failed\n");
		return fd;
	}

	// Allocate the space
	ftruncate(fd, HEAP_SIZE);

	// Map the shared memory object to the virtual address space
	if (shared_memory_map(fd, &__sym_heap) == SHM_ERROR_MAP) {
		perror("mmap\n");
		return SHM_ERROR_MAP;
	}

	// Zero out the heep
	memset(__sym_heap, 0, HEAP_SIZE);

	// Return success
	return SHM_ERROR_NONE;
}

/**
 * Open a shared memory object
 */
int	shared_memory_open(int pe)
{
	int fd;
	char name[255];

	// The filename for the shared memory object
	sprintf(name, "/%s-shmem-%d", USER_IDENTIFIER, pe);

	// Open the shared memory object
	fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	// Return the result
	return fd == -1 ? SHM_ERROR_OPEN : fd;
}

/**
 * Map a shared memory object into virtual memory space
 */
int shared_memory_map(int fd, void **ptr)
{
	*ptr = mmap(0, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	return ptr == MAP_FAILED ? SHM_ERROR_MAP : SHM_ERROR_NONE;
}

/**
 * Clean up any shared memory objects
 */
void shared_memory_free()
{
	// Unmap the symmetric heap from memory
	munmap(__sym_heap, HEAP_SIZE);
}

/**
 * Malloc the given size of bytes on the heap
 */
void* shared_memory_malloc(size_t bytes)
{
	// Get the address in the symmetric heap
	void* addr = __sym_heap + __heap_ptr;

	// Calculate the new offset
	__heap_ptr += bytes;

	// Return the calculated address
	return addr;
}
