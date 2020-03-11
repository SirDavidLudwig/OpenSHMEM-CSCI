#include "shared_mem.h"

/**
 * Generate the key to a shared memory object
 *
 * @param identifier The identifier for the key
 * @param key        The resulting key
 */
void shared_mem_key_int(int identifier, char key[256])
{
	sprintf(key, "%s-shmem-%d", SHARED_MEMORY_PREFIX, identifier);
}

/**
 * Generate the key to a shared memory object
 *
 * @param identifier The identifier for the key
 * @param key        The resulting key
 */
void shared_mem_key_str(char *identifier, char key[256])
{
	sprintf(key, "%s-shmem-%s", SHARED_MEMORY_PREFIX, identifier);
}

/**
 * Create or open a shared memory object and map it into memory
 *
 * @param key    The name of the shared memory object
 * @param size   The size of the shared memory object
 * @param region A pointer to the resulting region
 * @return       Returns the file descriptor upon success, otherwise -1
 */
int shared_mem_open(char *key, size_t size, void **region)
{
	int fd;

	// Open the shared memory object
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
 * @return       Returns the file descriptor upon success, otherwise -1
 */
int shared_mem_create(char *key, size_t size, void **region)
{
	int fd;

	// Open the shared memory object
	if (-1 == (fd = shm_open(key, O_CREAT | O_RDWR, 0666))) {
		perror("Open shared memory failed\n");
		return -1;
	}

	// Allocate the space
	ftruncate(fd, size);

	// Map the shared memory object to the virtual address space
	if (-1 == shared_mem_map(fd, size, region)) {
		perror("Failed to map memory object\n");
		return -1;
	}

	// Zero-out the heap
	memset(*region, 0, size);

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
 * Unmap the given shared memory region
 *
 * @param region The shared memory region to unmap
 * @param size   The size of the shared memory object
 */
void shared_mem_close(void *region, size_t size)
{
	// Unmap the memory region
	munmap(region, size);
}
