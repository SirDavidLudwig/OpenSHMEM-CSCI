#include "shared_memory.h"

// Private Members Data ----------------------------------------------------------------------------

/**
 * The symmetric heap
 */
void* __sym_heap;

/**
 * A pointer to the end of the symmetric heap
 */
int __heap_ptr;

// Private Member Functions ------------------------------------------------------------------------

/**
 * Create the shared memory object and allocate the requested bytes
 */
int	shm_create(size_t bytes, int pe)
{
	int fd;
	char name[255];

	sprintf(name, "/%d-shmem-%d", getpid(), pe);

	// Create the shared memory object
	fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (fd != -1) {
		// Allocate the size of the memory object
		ftruncate(fd, bytes);
	}

	// Return success
	return fd;
}

// Heap Management ---------------------------------------------------------------------------------

/**
 * Create and map a shared memory object. Returns -1 upon error
 */
int shm_init(int pe)
{
	int fd;
	int result;

	// Create and allocate the shared memory object
	fd = shm_create(HEAP_SIZE, pe);
	if (result == -1) {
		perror("Shared memory failed\n");
		return SHM_ERROR_CREATE;
	}

	// Map the shared memory object to the virtual address space
	__sym_heap = mmap(0, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (__sym_heap == MAP_FAILED)
	{
		perror("mmap\n");
		return SHM_ERROR_MAP;
	}

	// 0 out the heep
	memset(__sym_heap, 0, HEAP_SIZE);

	// Return success
	return SHM_ERROR_NONE;
}

/**
 * Clean up any shared memory objects
 */
void shm_free()
{
	// Unmap the symmetric heap from memory
	munmap(__sym_heap, HEAP_SIZE);
}

/**
 * Malloc the given size of bytes on the heap
 */
void* shm_malloc(size_t bytes)
{
	// Get the address in the symmetric heap
	void* addr = __sym_heap + __heap_ptr;

	// Calculate the new offset
	__heap_ptr += bytes;

	// Return the calculated address
	return addr;
}
