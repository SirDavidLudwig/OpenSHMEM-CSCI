#include "shared_mem.h"

/**
 * Create or open a shared memory object and map it into memory
 *
 * @param key    The name of the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns the file descriptor upno success, otherwise -1
 */
int shared_mem_open(char *key, size_t size, void **region)
{
	int fd;

	// Create and allocate the shared memory object
	if (-1 == (fd = shm_open(key, O_CREAT | O_RDWR, 0666))) {
		perror("Open shared memory failed\n");
		return -1;
	}

	// Map the shared memory object to the virtual address space
	if (-1 == shared_mem_map(fd, size, region)) {
		perror("Failed to map memory object\n");
		return -1;
	}

	// Return the file descriptor
	return fd;
}

/**
 * Open and map a shared memory object and truncate it
 *
 * @param key    The name of the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns the file descriptor upno success, otherwise -1
 */
int shared_mem_create(char *key, size_t size, void **region)
{
	int fd;

	// Open and map the region
	if (-1 == (fd = shared_mem_open(key, size, region))) {
		return -1;
	}

	// Allocate the space
	ftruncate(fd, size);

	// Return the file descriptor
	return fd;
}

/**
 * Map a shared memory object into virtual memory space
 *
 * @param fd     The file descriptor for the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns 0 for success, -1 for failure
 */
int shared_mem_map(int fd, size_t size, void **region)
{
	*region = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	return region == MAP_FAILED ? -1 : 0;
}

/**
 * Clean up any shared memory objects
 *
 * @param region The shared memory region to unmap
 * @param size   The size of the shared memory object
 */
void shared_mem_unmap(void *region, size_t size)
{
	// Unmap the symmetric heap from memory
	munmap(region, size);
}
