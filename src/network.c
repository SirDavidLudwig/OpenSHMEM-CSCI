#include "network.h"

// Private Data Members ----------------------------------------------------------------------------

/**
 * A reference to the network thread
 */
pthread_t __network_thread_id;

/**
 * Indicate if the network is ready
 */
int __network_ready;

/**
 * Indicate if the thread has been finalized
 */
int __network_finalized;

/**
 * Keep a global request index
 */
long __network_request_index;

// Private Structs ---------------------------------------------------------------------------------

struct network_args
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

/**
 * Used to quickly iterate over all existing sockets
 */
struct socket_channels **__sockets;

/**
 * Map a PE's ID to a socket
 */
struct socket_channels *__socket_map;

/**
 * Quickly create a TCP socket
 */
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
 * @todo Remove the continue comment
 * Wait for all clients to connect
 */
int __connect_clients(int my_pe, int n_pes, int *n_conns)
{
	printf("%d: Waiting for clients...\n", my_pe);
	struct socket_container *sock_cmd;
	struct socket_container *sock_data;
	int addrlen = sizeof(struct sockaddr_in);
	int pe;

	// Create the server sockets
	sock_cmd = __create_server_socket(n_pes, PORT + (my_pe << 1));
	sock_data = __create_server_socket(n_pes, PORT + (my_pe << 1) + 1);

	// Check the created sockets
	if (sock_cmd == NULL || sock_data == NULL) {
		perror("Failed to create server sockets\n");
		return 0;
	}

	// Use a RTE barrier to ensure that socket servers are created
	rte_barrier();

	// Loop through corresponding remote PEs
	for (pe = 1; pe < n_pes; pe += 2) {
		// No need for sockets with local processes
		if (rte_is_local_to(pe)) {
			// continue;
		}

		// Connect to the server via the command and data channels
		__socket_map[pe].fd_cmd = accept(sock_cmd->fd, (struct sockaddr *) &sock_cmd->address,
			(socklen_t *) &addrlen);
		__socket_map[pe].fd_data = accept(sock_data->fd, (struct sockaddr *) &sock_data->address,
			(socklen_t *) &addrlen);

		// Check the socket results
		if (__socket_map[pe].fd_cmd == 0 || __socket_map[pe].fd_data == 0) {
			return 0;
		}

		// Change the sockets to non-blocking
		fcntl(__socket_map[pe].fd_cmd, F_SETFL, O_NONBLOCK);
		fcntl(__socket_map[pe].fd_data, F_SETFL, O_NONBLOCK);

		// Increment the number of connections
		(*n_conns)++;
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
 * @todo Remove the continue comment
 * Connect to all other remote processes
 */
int __connect_servers(int my_pe, int n_pes, int *n_conns)
{
	printf("%d: Connecting to servers...\n", my_pe);

	// Use a RTE barrier to ensure that socket servers are created
	rte_barrier();

	// Loop through corresponding remote PEs
	for (int pe = 0; pe < n_pes; pe += 2) {
		// No need for sockets with local processes
		if (rte_is_local_to(pe)) {
			// continue;
		}

		// Connect to the server via the command and data channels
		__socket_map[pe].fd_cmd = __create_client_socket(rte_pe_host(pe), PORT + (pe << 1));
		__socket_map[pe].fd_data = __create_client_socket(rte_pe_host(pe), PORT + (pe << 1) + 1);

		// Check the socket results
		if (__socket_map[pe].fd_cmd == 0 || __socket_map[pe].fd_data == 0) {
			return 0;
		}

		// Change the sockets to non-blocking
		fcntl(__socket_map[pe].fd_cmd, F_SETFL, O_NONBLOCK);
		fcntl(__socket_map[pe].fd_data, F_SETFL, O_NONBLOCK);

		(*n_conns)++;
	}

	return 1;
}

/**
 * Connect to all other remote PEs
 */
int __connect(int pe, int n_pes, int *n_conns)
{
	int result;
	int i, j;

	// Create the list of sockets by PE
	__socket_map = malloc(n_pes * sizeof(struct socket_channels));
	memset(__socket_map, 0, n_pes * sizeof(struct socket_channels));

	// Connect the PEs. Odd PEs connect to even PEs
	*n_conns = 0;
	if (pe & 1) {
		result = __connect_servers(pe, n_pes, n_conns);
	} else {
		result = __connect_clients(pe, n_pes, n_conns);
	}

	// Exit if an error occurred
	if (result == 0) {
		return -1;
	}

	// Map valid socket references for quick iteration
	__sockets = malloc(*n_conns * sizeof(struct socket_channels*));
	for (i = j = 0; i < n_pes; i++) {
		if (__socket_map[i].fd_cmd != 0) {
			__sockets[j++] = &__socket_map[i];
		}
	}
}

/**
 * Disconnect all other remote PEs
 */
void __disconnect(int n_pes, int n_conns)
{
	int i;

	// Close all connections
	for (i = 0; i < n_pes; i++) {
		close(__socket_map[i].fd_cmd);
		close(__socket_map[i].fd_data);
	}

	// Free memory
	free(__sockets);
	free(__socket_map);
}

/**
 * Handle any incoming requests
 */
void __network_receive(int my_pe, int n_conns)
{
	struct packet header;
	ssize_t n_bytes;
	for (int i = 0; i < n_conns; i++) {
		// Attempt to receive a message. If no message is received, continue
		if ((n_bytes = recv(__sockets[i]->fd_cmd, &header, sizeof(header), 0)) < 1) {
			continue;
		} else {
			printf("Received from %d: num_bytes: %ld\n", header.origin, n_bytes);
			if (header.size > 0) {
				char *data = malloc(header.size);
				if (recv(__sockets[i]->fd_cmd, data, header.size, 0) != header.size) {
					perror("Corrupt packet received");
				}
				printf("Received data: %c\n", *data);

				// The worker should be responsible of freeing the data
				free(data);
			}
		}
	}
}

/**
 * Fulfill any completed requests
 */
void __network_respond(int my_pe, int n_conns)
{

}

/**
 * Handle the network communication
 */
void __network_run(int pe, int n_conns)
{
	// Mark the network as ready
	__network_ready = 1;

	printf("%d: Network running\n", pe);

	while (__network_finalized == 0) {
		// Receive any pending requests
		__network_receive(pe, n_conns);

		// Complete any finished requests
		__network_respond(pe, n_conns);

		// Done processing this iteration
		pthread_yield();
	}
}

/**
 * The main network thread
 */
void* __network_thread(void *argptr)
{
	struct network_args *args = (struct network_args*)argptr;
	int i, n_conns;

	// Connect to all other remote PEs
	if (__connect(args->pe, args->n_pes, &n_conns) == -1) {
		perror("Failed to connect sockets");
		return NULL;
	}

	// Run the network thread
	__network_run(args->pe, n_conns);

	// Disconnect all connections
	__disconnect(args->n_pes, n_conns);

	// Free memory
	free(argptr);

	return NULL;
}

// Layer Management --------------------------------------------------------------------------------

/**
 * Initialize the network thread
 */
void network_init()
{
	struct network_args *args = malloc(sizeof(struct network_args));
	args->pe = rte_my_pe();
	args->n_pes = rte_n_pes();
	__network_ready = 0;
	__network_finalized = 0;
	__network_request_index = 0;
	pthread_create(&__network_thread_id, NULL, __network_thread, args);
}

/**
 * Check if the network is ready for communication
 */
int network_is_ready()
{
	return __network_ready == 1;
}

/**
 * Finalize the network thread
 */
void network_finalize()
{
	__network_ready = 0;
	__network_finalized = 1;
	pthread_join(__network_thread_id, NULL);
}

// Interface ---------------------------------------------------------------------------------------

/**
 * Send a value to a remote process
 *
 * @param dest   The offset position relative to the beginning of the symmetric heap
 * @param source A reference to the variable to send
 * @param bytes  The number of bytes to send
 * @param pe     The destination PE
 */
void network_put(int dest, const void *source, size_t bytes, int my_pe, int dest_pe)
{
	size_t packet_size = sizeof(struct packet) + bytes;
	struct packet *p = malloc(packet_size);
	p->handler = 20;
	p->origin = my_pe;
	p->request_index = __network_request_index++;
	p->size = bytes;
	p->heap_offset = dest;

	// Copy the data into the packet
	memcpy(p->data, source, bytes);

	// printf("%d: Sending message of size %ld to %d:\nhandler: %d\norigin: %d\nrequest ID: %ld\nsize: %ld\noffset: %ld\n\n",
	// 	my_pe, sizeof(*p), dest_pe, p->handler, p->origin, p->request_index, p->size, p->heap_offset);

	// Send the packet to the remote PE
	send(__socket_map[dest_pe].fd_cmd, p, packet_size, 0);

	// Free the packet from memory
	free(p);
}
