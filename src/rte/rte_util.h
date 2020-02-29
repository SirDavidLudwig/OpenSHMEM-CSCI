#include "rte.h"

/**
 * The maximum hostname length
 */
#define HOSTNAME_LEN 256

/**
 * Create a map of all hostnames for the PEs
 */
char** rte_map_hosts();
