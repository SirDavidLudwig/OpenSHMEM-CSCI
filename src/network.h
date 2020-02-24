#pragma once

#include <pthread.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sched.h>
#include <fcntl.h>

#include "rte.h"
#include "shared_memory.h"

#define PORT 31500

/**
 * The packet header for active messages
 */
struct packet_header
{
	char handler;
	int origin;
	long request_index;
	size_t size;
	long heap_offset;
} __attribute__((packed));

/**
 * The complete packet for active messages
 */
struct packet
{
	char handler;
	int origin;
	long request_index;
	size_t size;
	long heap_offset;
	char data[];
} __attribute__((packed));

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the network thread
 */
void network_init();

/**
 * Check if the network is ready for communication
 */
int network_is_ready();

/**
 * Finalize the network thread
 */
void network_finalize();

// Interface ---------------------------------------------------------------------------------------

/**
 * Send a value to a remote process
 *
 * @param dest   The offset position relative to the beginning of the symmetric heap
 * @param source A reference to the variable to send
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void network_put(int dest, const void *source, size_t bytes, int my_pe, int dest_pe);
