#include "shmem.h"

#include <stddef.h>
#include <stdio.h>

#include "comm/comm.h"
#include "memory/shared_heap.h"
#include "rte/rte.h"

// Library Setup/Querying --------------------------------------------------------------------------

/**
 * Map the locality of the nodes in the comm layer
 */
static void __shmem_map_locality()
{
	char hostname[HOSTNAME_LEN];
	char **hosts;
	int **pes;
	int n_hosts;
	int *n_pes;
	int i;

	// Get the host name of the current node
	rte_hostname(hostname, NULL);

	// Generate the list of host-PE mappings
	rte_remote_peers(&hosts, &pes, &n_hosts, &n_pes);

	// Map everything out in the communication layer
	comm_map_locality(hostname, hosts, pes, n_hosts, n_pes);

	// Free memory
	for (i = 0; i < n_hosts; i++) {
		free(hosts[i]);
		free(pes[i]);
	}
	free(hosts);
	free(pes);
	free(n_pes);
}

/**
 * [Collective]
 * Allocates and initializes resources used by OpenSHMEM
 */
void shmem_init()
{
	// Initialize the runtime layer
	rte_init();

	// Initialize the communication layer
	comm_init(rte_my_local_pe(), rte_n_local_pes(), rte_my_pe(), rte_n_pes(), rte_n_nodes());

	// Have the local root map the locality of nodes
	if (0 == rte_my_local_pe()) {
		__shmem_map_locality();
	}

	// Wait for all processes to initialize their comm layer
	rte_barrier_all();

	// Wire up all PE communication
	comm_wireup();

	// Wait for all processes to finish
	shmem_sync_all();
}

/**
 * Returns the PE number for the calling PE
 */
int shmem_my_pe()
{
	return rte_my_pe();
}

/**
 * Get the number of PEs running in a program
 */
int shmem_n_pes()
{
	return rte_n_pes();
}

/**
 * [Collective]
 * Release all resources used by OpenSHMEM
 */
void shmem_finalize()
{
	// Wait for other processes
	shmem_sync_all();

	// Close tho communication layer
	comm_finalize();

	// Close MPI
	rte_finalize();
}

// Remote Memory Access ----------------------------------------------------------------------------

#define SHMEM_OP(type, typename, op, suffix)                                                    \
	void shmem_##typename##_##op##suffix(type *dest, const type *source, size_t nelems, int pe) \
	{                                                                                           \
		shmem_##op##mem##suffix((void*) dest, (const void*) source, nelems*sizeof(type), pe);   \
	}

#define SHMEM_OPS(type, typename)       \
	SHMEM_OP(type, typename, get, )     \
	SHMEM_OP(type, typename, put, )     \
	SHMEM_OP(type, typename, get, _nbi) \
	SHMEM_OP(type, typename, put, _nbi)

SHMEM_OPS(float, float)
SHMEM_OPS(double, double)
SHMEM_OPS(long double, longdouble)
SHMEM_OPS(char, char)
SHMEM_OPS(signed char, schar)
SHMEM_OPS(short, short)
SHMEM_OPS(int, int)
SHMEM_OPS(long, long)
SHMEM_OPS(long long, longlong)
SHMEM_OPS(unsigned char, uchar)
SHMEM_OPS(unsigned short, ushort)
SHMEM_OPS(unsigned int, uint)
SHMEM_OPS(unsigned long, ulong)
SHMEM_OPS(unsigned long long, ulonglong)
SHMEM_OPS(int8_t, int8)
SHMEM_OPS(int16_t, int16)
SHMEM_OPS(int32_t, int32)
SHMEM_OPS(int64_t, int64)
SHMEM_OPS(uint8_t, uint8)
SHMEM_OPS(uint16_t, uint16)
SHMEM_OPS(uint32_t, uint32)
SHMEM_OPS(uint64_t, uint64)
SHMEM_OPS(size_t, size)
SHMEM_OPS(ptrdiff_t, ptrdiff)

/**
 * Copy data from the specified PE
 */
void shmem_getmem(void *dest, const void *source, size_t nelems, int pe)
{
	comm_get(dest, source, nelems, pe);
}

/**
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_putmem(void *dest, const void *source, size_t nelems, int pe)
{
	comm_put(dest, source, nelems, pe);
}

/**
 * [Nonblocking]
 * Copy data from the specified PE
 */
void shmem_getmem_nbi(void *dest, const void *source, size_t nelems, int pe)
{
	//
}

/**
 * [Nonblocking]
 * Copy data from a contiguous local data object to a data object on the specified PE
 */
void shmem_putmem_nbi(void *dest, const void *source, size_t nelems, int pe)
{
	//
}

// Collectives -------------------------------------------------------------------------------------

/**
 * @todo Use MPI barriers for testing
 * [Collective]
 * Blocks the PE until all other PEs arrive at the barrier
 */
void shmem_barrier_all()
{
	rte_barrier_all();
}

/**
 * [Collective]
 * Barrier and flush all current requests
 */
void shmem_sync_all()
{
	shmem_barrier_all();
	shmem_quiet();
}

// Memory Management -------------------------------------------------------------------------------

/**
 * [Collective]
 * Returns a pointer to a block of at least `size` bytes suitably aligned for any use
 */
void* shmem_malloc(size_t size)
{
	struct shared_heap_t *heap;
	void *address;

	// Get the symmetric heap
	heap = comm_symmetric_heap();

	// Allocate the requested memory
	address = shared_heap_malloc(heap, size);

	// Wait on other processes
	shmem_sync_all();

	// Return the allocated address
	return address;
}

/**
 * [Collective]
 * Free up a block of memory
 */
void shmem_free(void *ptr)
{
	struct shared_heap_t *heap;

	// Get the symmetric heap
	heap = comm_symmetric_heap();

	// Free the given address
	shared_heap_free(heap, ptr);

	// Wait an other processes
	shmem_sync_all();
}

// Memory Ordering ---------------------------------------------------------------------------------

/**
 * Ensures completion of Put, AMO, memory store, and nonblocking Put and Get routines
 */
void shmem_quiet()
{
	comm_flush();
}
