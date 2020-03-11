#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

/**
 * Resolve a hostname to an address
 *
 * @param hostname The hostname to resolve
 * @return         The resolved address upon success, otherwise 0
 */
in_addr_t netutil_resolve_host(char *hostname);
