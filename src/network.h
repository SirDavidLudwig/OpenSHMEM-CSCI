#pragma once

#include <pthread.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sched.h>

#include "rte.h"
#include "shared_memory.h"

#define PORT 31500

/**
 * The packet used for active messages
 */
struct packet
{
	char handler;
	int origin;
	long request_index;
	long size;
	long heap_offset;
	char *data;
}__attribute__((packed));

/**
 * Initialize the network thread
 */
void network_init();

/**
 * Finalize the network thread
 */
void network_finalize();
