#include "comm_remote.h"

/**
 * A reference to the server socket
 */
struct server_t *__server_cmd;
struct server_t *__server_data;

/**
 * Initialize remote communication
 */
void comm_remote_init(int my_pe, int n_pes)
{
	if (my_pe & 1 == 0) {
		//__server_data = network_create_server();
		//__server_cmd = network_create_server();
	}
}

/**
 * Wire up remote processes
 */
void comm_remote_wireup(int my_pe, char **hostmap)
{

}
