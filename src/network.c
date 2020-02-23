#include "network.h"

pthread_t __thread_id;

struct comm_args
{
	int pe;
	int n_pes;
};

/**
 * A simple socket container
 */
struct socket_container
{
	int fd;
	struct sockaddr_in address;
};

/**
 * Store each socket channel in a single container
 */
struct socket_channels
{
	int fd_cmd;
	int fd_data;
};

// Network Thread ----------------------------------------------------------------------------------

struct socket_channels *__sockets;

struct socket_container* __create_socket(in_addr_t host, int port)
{
	struct socket_container *sock = malloc(sizeof(struct socket_container));

	memset(&sock->address, 0, sizeof(sock->address));
	sock->address.sin_family = AF_INET;
	sock->address.sin_addr.s_addr = host;
	sock->address.sin_port = htons(port);

	if ((sock->fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		return NULL;
	}

	return sock;
}

/**
 * Create a socket server
 */
struct socket_container* __create_server_socket(int n_pes, int port)
{
	struct socket_container *sock = __create_socket(INADDR_ANY, port);
	int opt = 1;

    if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("sockopt failed");
        return NULL;
    }

	if (bind(sock->fd, (struct sockaddr *) &sock->address, sizeof(sock->address)) < 0) {
		perror("bind failed");
		return NULL;
	}

	if (listen(sock->fd, n_pes) < 0) {
		perror("listen failed:");
		return NULL;
	}

	return sock;
}

/**
 * Create a client socket
 */
int __create_client_socket(char *hostname, int port)
{
	int sockfd;
	struct socket_container *sock = __create_socket(INADDR_ANY, port);
	struct hostent *host = gethostbyname(hostname);

	// Copy the resolved IP address into the socket address
	memcpy(&sock->address.sin_addr, host->h_addr_list[0], host->h_length);

	// Connect to the server
	if (connect(sock->fd, (struct sockaddr *) &sock->address, sizeof(sock->address)) < 0) {
		perror("Could not connect to host");
		return 0;
	}

	// Keep the socket descriptor
	sockfd = sock->fd;

	// Free the socket container
	free(sock);

	// Return the descriptor
	return sockfd;
}

/**
 * Wait for all clients to connect
 */
int __connect_clients(int my_pe, int n_pes)
{
	printf("%d: Waiting for clients...\n", my_pe);
	struct socket_container *sock_cmd;
	struct socket_container *sock_data;
	int addrlen = sizeof(struct sockaddr_in);
	int pe;

	// Create the server sockets
	sock_cmd = __create_server_socket(n_pes, PORT + (my_pe << 1));
	sock_data = __create_server_socket(n_pes, PORT + (my_pe << 1) + 1);

	// // Check the created sockets
	if (sock_cmd == NULL || sock_data == NULL) {
		perror("Failed to create server sockets\n");
		return 0;
	}

	// Loop through corresponding remote PEs
	for (pe = ~my_pe & 1; pe < n_pes; pe += 2) {
		// No need for sockets with local processes
		if (rte_is_local_to(pe)) {
			// continue;
		}
		// Connect to the server via the command and data channels
		__sockets[pe].fd_cmd = accept(sock_cmd->fd, (struct sockaddr *) &sock_cmd->address,
			(socklen_t *) &addrlen);
		__sockets[pe].fd_data = accept(sock_data->fd, (struct sockaddr *) &sock_data->address,
			(socklen_t *) &addrlen);

		// Check the socket results
		if (__sockets[pe].fd_cmd == 0 || __sockets[pe].fd_data == 0) {
			return 0;
		}
	}

	// Close the server sockets
	close(sock_cmd->fd);
	close(sock_data->fd);

	// Free the socket memory
	free(sock_cmd);
	free(sock_data);

	return 1;
}

/**
 * Connect to all other remote processes
 */
int __connect_servers(int my_pe, int n_pes)
{
	// Loop through corresponding remote PEs
	for (int pe = ~my_pe & 1; pe < n_pes; pe += 2) {
		// No need for sockets with local processes
		if (rte_is_local_to(pe)) {
			// continue;
		}

		// Connect to the server via the command and data channels
		__sockets[pe].fd_cmd = __create_client_socket(rte_pe_host(pe), PORT + (pe << 1));
		__sockets[pe].fd_data = __create_client_socket(rte_pe_host(pe), PORT + (pe << 1) + 1);

		// Check the socket results
		if (__sockets[pe].fd_cmd == 0 || __sockets[pe].fd_data == 0) {
			return 0;
		}
	}

	return 1;
}

/**
 * Connect to all other remote PEs
 */
int __connect(int pe, int n_pes)
{
	// Connect the PEs. Odds connect to events first
	if (pe & 1) {
		return __connect_servers(pe, n_pes) && __connect_clients(pe, n_pes);
	}
	return __connect_clients(pe, n_pes) && __connect_servers(pe, n_pes);
}

void __run()
{

}

/**
 * The main network thread
 */
void* network_thread(void *argptr)
{
	struct comm_args *args = (struct comm_args*)argptr;
	int i;

	// Create the list of sockets by PE
	__sockets = malloc(args->n_pes * sizeof(struct socket_channels));
	memset(__sockets, 0, args->n_pes * sizeof(struct socket_channels));

	// Connect to all other remote PEs
	if (__connect(args->pe, args->n_pes) == 0) {
		return NULL;
	}

	/**
	 * @todo remove later after more testing
	 */
	printf("%d: network ready\n", args->pe);

	// Run the network thread
	__run();

	// Close all connections
	for (i = 0; i < args->n_pes; i++) {
		close(__sockets[i].fd_cmd);
		close(__sockets[i].fd_data);
	}

	// // Free memory
	free(__sockets);
	free(argptr);

	return NULL;
}

// Interface Functions -----------------------------------------------------------------------------

/**
 * Initialize the network thread
 */
void network_init()
{
	struct comm_args *args = malloc(sizeof(struct comm_args));
	args->pe = rte_my_pe();
	args->n_pes = rte_n_pes();
	pthread_create(&__thread_id, NULL, network_thread, args);
}

/**
 * Finalize the network thread
 */
void network_finalize()
{
	pthread_join(__thread_id, NULL);
}
