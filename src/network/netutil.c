#include "netutil.h"

/**
 * Resolve a hostname to an address
 *
 * @param hostname The hostname to resolve
 * @return         The resolved address upon success, otherwise 0
 */
in_addr_t netutil_resolve_host(char *hostname)
{
	struct hostent *host;
	int i;

	// Resolve the hostname
	if (NULL == (host = gethostbyname(hostname))) {
		fprintf(stderr, "Failed to resolve hostname: %s\n", hostname);
		return 0;
	}

	// Grab the first non-null result
	for (int i = 0; host->h_addr_list[i] != NULL; i++) {
		return ((struct in_addr**) host->h_addr_list)[i]->s_addr;
	}

	return 0;
}
