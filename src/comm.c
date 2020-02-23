#include "comm.h"

void **__shared_mem;

/**
 * Initialize shared memory communication
 */
void __comm_init_shared_memory()
{
	int fd;
	int n_pes = rte_n_pes();

	// Allocate the shared memory pointers
	__shared_mem = malloc(n_pes * sizeof(void*));
	memset(__shared_mem, 0, n_pes * sizeof(void*));

	// Find local processes and map their shared memory
	for (int i = 0; i < n_pes; i++) {
		if (rte_is_local_to(i)) {
			if (SHM_ERROR_OPEN == (fd = shared_memory_open(i))) {
				printf("%d: Failed to open shared memory for PE %d\n", rte_my_pe(), i);
				return;
			}
			if (SHM_ERROR_MAP == shared_memory_map(fd, &__shared_mem[i])) {
				printf("%d: Failed to map shared memory for PE %d\n", rte_my_pe(), i);
				return;
			}
		}
	}
}

/**
 * Initialize the communication layer
 */
void comm_init()
{
	__comm_init_shared_memory();
}

/**
 * Finalize the communication layer
 */
void comm_finalize()
{
	for (int i = 0; i < rte_n_pes(); i++) {
		if (__shared_mem[i] != 0) {
			shared_memory_free(i);
		}
	}
}

/**
 * Get a value from another process
 *
 * @param dest   The offset position relative to the beginning of the symmetric heap
 * @param source A reference to the variable to send
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_get(void *dest, const int source, size_t bytes, int pe)
{
	if (__shared_mem[pe]) {
		memcpy(dest, __shared_mem[pe] + source, bytes);
	}
}

/**
 * Send a value from another process
 *
 * @param dest   The offset position relative to the beginning of the symmetric heap
 * @param source A reference to the variable to send
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_put(int dest, const void *source, size_t bytes, int pe)
{
	if (__shared_mem[pe]) {
		memcpy(__shared_mem[pe] + dest, source, bytes);
	}
}
