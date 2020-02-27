#include "comm.h"

// Private Data Members ----------------------------------------------------------------------------

/**
 * A reference to the current PE
 */
static int __pe;

/**
 * A reference to the number of PEs
 */
static int __n_pes;

/**
 * A map of all PE's hostnames, indexed by PE ID
 */
static char **__pe_hosts;

/**
 * Store all references to shared memory
 */
static void **__shared_mem;

// Internal Functions ------------------------------------------------------------------------------

/**
 * Initialize shared memory communication
 */
void __comm_init_shared_memory()
{
	int fd;

	// Allocate the shared memory pointers
	__shared_mem = malloc(__n_pes * sizeof(void*));
	memset(__shared_mem, 0, __n_pes * sizeof(void*));

	// Find local processes and map their shared memory
	for (int i = 0; i < __n_pes; i++) {
		if (comm_is_local(i)) {
			if (SHM_ERROR_OPEN == (fd = shared_memory_open(i))) {
				printf("%d: Failed to open shared memory for PE %d\n", __pe, i);
				return;
			}
			if (SHM_ERROR_MAP == shared_memory_map(fd, &__shared_mem[i])) {
				printf("%d: Failed to map shared memory for PE %d\n", __pe, i);
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
	for (int i = 0; i < __n_pes; i++) {
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
 *
 * @param pe          The current PE's ID
 * @param n_pes       The number of PEs
 * @param pe_host_map A map of all PE's hostnames by PE ID
 */
void comm_init(int pe, int n_pes, char **pe_host_map)
{
	__pe = pe;
	__n_pes = n_pes;
	__pe_hosts = pe_host_map;

	// Connect the local PEs via shared memory
	__comm_init_shared_memory();
	__comm_init_threads();
}

/**
 * Wire up the PE communication layer
 */
void comm_wireup()
{

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
 * Determine if the given PE is local to the current PE
 */
int comm_is_local(int pe)
{
	return comm_are_local(__pe, pe);
}

/**
 * Determine if a PE is local to another PE
 */
int comm_are_local(int pe1, int pe2)
{
	return strcmp(__pe_hosts[pe1], __pe_hosts[pe2]) == 0;
}

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
