#include "comm_remote.h"

/**
 * True modulus
 */
#define MOD(a, b) (((a % b) + b) % b)

/**
 * The current PE's ID
 */
static int __my_pe;

/**
 * The total number of PEs
 */
static int __n_pes;

/**
 * The number of remote PEs
 */
static int __n_remote_pes;

/**
 * A reference to the server socket
 */
static socket_t *__server_cmd;
static socket_t *__server_data;

/**
 * The list of sockets
 */
socket_t *__sockets_cmd = NULL;
socket_t *__sockets_data = NULL;

/**
 * Open up the server to connect clients
 */
static void __comm_remote_open_servers()
{
	__server_cmd = network_create_server(PORT_BASE + 2*__my_pe, __n_pes);
	__server_data = network_create_server(PORT_BASE + 2*__my_pe + 1, __n_pes);
}

/**
 * Close the server
 */
static void __comm_remote_close_servers()
{
	if (__server_cmd) {
		network_close(__server_cmd);
		network_close(__server_data);
		__server_cmd = __server_data = NULL;
	}
}

/**
 * Wire up the client to the servers
 */
static void __comm_remote_wire_client(int stride)
{
	locality_map_t *map;
	socket_t *sock_cmd;
	socket_t *sock_data;
	int pe;
	int i;

	pe = __my_pe - (stride >> 1);
	for (i = 0; i < __n_pes; i += stride, pe += stride) {
		// Loop around to beginning of list
		if (pe >= __n_pes) {
			pe = 0;
		}

		// Skip local nodes
		map = comm_node_map(MOD(pe, __n_pes));
		if (map->type == PE_TYPE_LOCAL) {
			continue;
		}

		sock_cmd = &__sockets_cmd[map->index];
		sock_data = &__sockets_data[map->index];

		// Connect to the server
		network_init_socket(sock_cmd);
		network_init_socket(sock_data);

		if (0 == network_connect(sock_cmd, map->host->addr, PORT_BASE + 2*pe)) {
			perror("Failed to connect");
			return;
		}
		network_send(sock_cmd, &__my_pe, sizeof(int));
		network_connect(sock_data, map->host->addr, PORT_BASE + 2*pe + 1);
	}
}

/**
 * Wire up clients by accepting incoming connections
 */
static void __comm_remote_wire_server(int stride)
{
	locality_map_t *map;
	socket_t sock;
	int pe;
	int i;

	for (i = (__my_pe & 1) + (stride >> 1); i < __n_pes; i += stride) {
		// Skip local nodes
		if (comm_node_map(i)->type == PE_TYPE_LOCAL) {
			continue;
		}

		// Accept the incoming connection
		network_accept_2(__server_cmd, &sock);
		network_receive(&sock, &pe, sizeof(int));
		map = comm_node_map(pe);
		memcpy(&__sockets_cmd[map->index], &sock, sizeof(socket_t));
		network_accept_2(__server_data, &__sockets_data[map->index]);
	}
}

/**
 * Initialize remote communication
 */
void comm_remote_init(int my_pe, int n_pes, int n_remote_pes)
{
	// Store the PE information
	__my_pe        = my_pe;
	__n_pes        = n_pes;
	__n_remote_pes = n_remote_pes;

	__comm_remote_open_servers();
}

/**
 * Finalize the remote communication layer
 */
void comm_remote_finalize()
{
	__comm_remote_close_servers();
}

/**
 * Wire up remote processes
 */
void comm_remote_wireup()
{
	int levels = ceil(log2(__n_pes));
	int index = 0;

	// Allocate memory for the sockets
	__sockets_cmd = malloc(__n_remote_pes * sizeof(socket_t));
	__sockets_data = malloc(__n_remote_pes * sizeof(socket_t));

	// Wire up the sockets
	for (int level = 0; level < levels; level++) {
		if ((__my_pe >> level) & 1) {
			__comm_remote_wire_client(2 << level);
		} else {
			__comm_remote_wire_server(2 << level);
		}
	}
	__comm_remote_close_servers();
}
