#include "comm.h"

// Private Data Members ----------------------------------------------------------------------------

/**
 * Store all references to shared memory
 */
void **__shared_mem;

// Internal Functions ------------------------------------------------------------------------------

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
 * Close any mapped shared memory
 */
void __comm_finalize_shared_memory()
{
	// Unmap shared memory
	for (int i = 0; i < rte_n_pes(); i++) {
		if (__shared_mem[i] != 0) {
			shared_memory_free(i);
		}
	}

	// Free the used memory
	free(__shared_mem);
}

/**
 * Create the network and worker threads
 */
void __comm_init_threads()
{
	network_init();
	worker_init();

	// Wait for the network and worker threads to initialize
	while (!network_is_ready() || !worker_is_ready());
}

/**
 * Finalize the network and worker threads
 */
void __comm_finalize_threads()
{
	network_finalize();
	worker_finalize();
}

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the communication layer
 */
void comm_init()
{
	__comm_init_shared_memory();
	__comm_init_threads();
}

/**
 * Finalize the communication layer
 */
void comm_finalize()
{
	// Wait for threads to finish
	__comm_finalize_threads();
	__comm_finalize_shared_memory();
}

// Interface ---------------------------------------------------------------------------------------

/**
 * Get a value from another process
 *
 * @param dest   The offset position relative to the beginning of the symmetric heap
 * @param source A reference to the variable to send
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_get(void *dest, const int source, size_t bytes, int my_pe, int dest_pe)
{
	// If the process is local, use shared memory
	if (__shared_mem[dest_pe]) {
		memcpy(dest, __shared_mem[dest_pe] + source, bytes);
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
void comm_put(int dest, const void *source, size_t bytes, int my_pe, int dest_pe)
{
	// If the process is local, use shared memory
	// if (__shared_mem[dest_pe]) {
	// 	memcpy(__shared_mem[dest_pe] + dest, source, bytes);
	// }
	network_put(dest, source, bytes, my_pe, dest_pe);
}

/**
 * [Blocking]
 * Wait for the worker thread to flush requests
 */
void comm_flush()
{
	//
}
