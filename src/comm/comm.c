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
 */
void comm_init(int my_pe, int n_pes)
{
	__my_pe = my_pe;
	__n_pes = n_pes;

	comm_local_init(my_pe);
	comm_remote_init(my_pe, n_pes);
}

/**
 * Finalize the communication layer
 */
void comm_finalize()
{
	comm_local_finalize(__n_pes);
}

/**
 * Wire up the processes
 */
void comm_wireup(char **hostmap)
{
	comm_local_wireup(__my_pe, __n_pes, hostmap);
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
		comm_local_get(dest_pe, dest, OFFSET(comm_local_heap(), source), bytes);
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
		comm_local_put(dest_pe, OFFSET(comm_local_heap(), dest), source, bytes);
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
