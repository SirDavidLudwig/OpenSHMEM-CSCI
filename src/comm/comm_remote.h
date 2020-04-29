#pragma once

#include <math.h>
#include <pthread.h>

#include "../common.h"
#include "../network/network.h"
#include "../rte/rte.h"
#include "comm_node.h"

#define HANDLER_PUT 0
#define HANDLER_GET 1

/**
 * The active message packet
 */
typedef struct packet
{
	char handler;       // The type of operation
	int origin;         // The sending PE
	long request_index; // The request index
	size_t size;        // The size of the packet's data
	long heap_offset;   // The offset into the symmetric heap
	char data[];
} __attribute__((packed)) packet_t;

/**
 * A packet container used for queues
 */
typedef struct packet_queue
{
	packet_t *packet;          // A pointer to the packet to send
	int pe_index;              // The destination PE
	struct packet_queue *next; // The next packet in the queue
} packet_queue_t;

/**
 * Initialize remote communication
 */
void comm_remote_init(int my_pe, int n_pes, int n_remote_pes);

/**
 * finalize the remote communication layer
 */
void comm_remote_finalize();

/**
 * Wire up remote processes
 */
void comm_remote_wireup();

/**
 * Start the network threads
 */
void comm_remote_start();

// Communication Methods ---------------------------------------------------------------------------

/**
 * Get a value from a remote PE
 *
 * @param pe   The PE to communicate with
 * @param dest The destination variable to put the result
 * @param src  The source offset position within the heap
 * @param size The number of bytes to send
 */
void comm_remote_get(int pe, void *dest, long src, size_t size);

/**
 * Put a value into a remote PE
 *
 * @param pe   The PE to communicate with
 * @param dest The destination offset position within the heap
 * @param src  The source variable to send
 * @param size The number of bytes to send
 */
void comm_remote_put(int pe, long dest, const void *src, size_t size);

// Communication Thread Methods --------------------------------------------------------------------
