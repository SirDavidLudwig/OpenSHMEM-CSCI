#include "comm.h"

#define MEM_PREFIX "dwl2x"
#define HEAP_SIZE  2 << 25

/**
 * The symmetric heap
 */
static struct heap_t *__symmetric_heap;

// Internal Functions ------------------------------------------------------------------------------

/**
 * Initialize the shared heap
 *
 * @param pe       The current process ID
 * @param size     The size of the heap
 * @param truncate Indicate if the heap should be truncated
 * @return         A reference to the heap
 */
static struct heap_t* __open_shared_heap(int pe, size_t size, int truncate)
{
	char key[255];

	// Create the key
	sprintf(key, "%s-shmem-%d", MEM_PREFIX, pe);

	// Create the shared heap object
	if (truncate) {
		return shared_heap_create(key, size);
	}
	return shared_heap_open(key, size);
}

/**
 * Free the shared heap
 */
static void __close_shared_heap(struct heap_t *heap)
{
	// Close the shared heap
	shared_heap_close(heap);
}

// Interface ---------------------------------------------------------------------------------------

/**
 * Initialize the communication layer
 */
void comm_init(int my_pe, int n_pes)
{
	// Create the shared heap object
	__symmetric_heap = __open_shared_heap(my_pe, HEAP_SIZE, 1);
}

/**
 * Finalize the communication layer
 */
void comm_finalize()
{
	__close_shared_heap(__symmetric_heap);
}

/**
 * Wire up the processes
 */
void comm_wireup(char **hostmap)
{
	//
}

// Accessors ---------------------------------------------------------------------------------------

/**
 * Get the symmetric heap
 *
 * @return A reference to the symmetric heap
 */
struct heap_t* comm_symmetric_heap()
{
	return __symmetric_heap;
}
