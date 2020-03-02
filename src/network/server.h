#pragma once

#include "network.h"

/**
 * A server socket wrapper
 */
struct server_t
{
	int fd_cmd; // File descriptor
	int fd_data;
};

