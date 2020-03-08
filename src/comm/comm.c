#include "comm.h"

/**
 * Calculate a variable's offset relative to the beginning of the symmetric heap
 */
#define OFFSET(a,b) ((long)b - (long)(a->heap->ptr))

/**
 * The current PE's ID
 */
static int __my_pe;

/**
 * The total number of PEs
 */
static int __n_pes;

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the communication layer
 *
 * @param my_local_pe The local ID of this PE with respect to the node
 * @param n_local_pes The total number of PEs on this node
 */
void comm_init(int my_local_pe, int n_local_pes)
{
	comm_local_init(my_local_pe, n_local_pes);
	//comm_remote_init();
}

/**
 * Finalize the communication layer
 */
void comm_finalize()
{
	comm_local_finalize();
}

/**
 * Wire up the processes
 */
void comm_wireup(int my_pe, int n_pes)
{
	comm_local_wireup();
	// comm_remote_wireup(hostmap);
}

// Communication Methods ---------------------------------------------------------------------------

/**
 * Get a value from another process
 *
 * @param dest   The destination to store the result
 * @param source The remote variable to get from
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_get(void *dest, const void *source, size_t bytes, int dest_pe)
{
	if (comm_is_local(dest_pe)) {
		comm_local_get(dest_pe, dest, comm_local_offset(source), bytes);
	}
}

/**
 * Send a value from another process
 *
 * @param dest   The destination to store the result
 * @param source The local variable to send
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void comm_put(void *dest, const void *source, size_t bytes, int dest_pe)
{
	if (comm_is_local(dest_pe)) {
		comm_local_put(dest_pe, comm_local_offset(dest), source, bytes);
	}
}

/**
 * [Blocking]
 * Wait for the worker thread to flush requests
 */
void comm_flush()
{

}

// Accessors ---------------------------------------------------------------------------------------

/**
 * Get the symmetric heap
 *
 * @return A reference to the symmetric heap
 */
struct shared_heap_t* comm_symmetric_heap()
{
	return comm_local_heap();
}

/**
 * Determine if the given PE is local to the current PE
 *
 * @param  pe The other PE to check
 * @return 1 if the PE is local, otherwise 0
 */
int comm_is_local(int pe)
{
	return comm_local_has(pe);
}
