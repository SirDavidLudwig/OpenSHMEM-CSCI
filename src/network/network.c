#include "network.h"

/**
 * Initialize an existing socket object
 *
 * @param sock The socket to initialize
 * @return     1 for success; otherwise 0
 */
int network_init_socket(socket_t *sock)
{
	int opt = 1;

	// Configure the initial address information
	memset(&sock->addr, 0, sizeof(sock->addr));
	sock->addr.sin_family      = AF_INET;
	sock->addr.sin_addr.s_addr = INADDR_ANY;

	// Create the socket
	if ((sock->fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed\n");
		return 0;
	}
	return 1;
}

/**
 * Create a socket
 *
 * @return The created socket
 */
socket_t* network_create_socket()
{
	socket_t *sock;
	int opt = 1;

	// Create the socket container
	sock = malloc(sizeof(socket_t));

	// Configure the initial address information
	if (0 == network_init_socket(sock)) {
		free(sock);
		return NULL;
	}

	// Return the socket
	return sock;
}

/**
 * Create a server socket
 *
 * @param port     The port to bind to
 * @param n_listen The maximum number of connections to listen for
 * @return         The created server socket
 */
socket_t* network_create_server(int port, int n_listen)
{
	socket_t *sock;
	int opt = 1;

	// Create the socket
	if (NULL == (sock = network_create_socket(port))) {
		perror("Failed to create socket server");
		return NULL;
	}

	// Allow the socket to be reuesed
	if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
	// if (setsockopt(sock->fd, SOL_SOCKET, 0, &opt, sizeof(opt))) {
		perror("Socket sockopt failed");
		return NULL;
	}

	// Bind the socket to the given port
	sock->addr.sin_port = htons(port);
	if (bind(sock->fd, (struct sockaddr*) &sock->addr, sizeof(sock->addr)) < 0) {
		perror("Socket bind failed");
		return NULL;
	}

	// Set the maximum number of connections to listen for
	if (listen(sock->fd, n_listen) < 0) {
		perror("Socket listen failed");
		return NULL;
	}

	return sock;
}

/**
 * Accept a connection if available
 *
 * @param server The server to accept from
 * @return       The newly connected socket
 */
socket_t* network_accept(socket_t *server)
{
	socket_t *sock;

	// Create the socket
	sock = malloc(sizeof(socket_t));

	// Accept the incoming connection
	if (0 == network_accept_2(server, sock)) {
		return NULL;
	}

	return sock;
}

/**
 * Accept a connection if available
 *
 * @param server The server to accept from
 * @param sock   The socket to store the result
 * @return       1 for success; otherwise 0
 */
int network_accept_2(socket_t *server, socket_t *sock)
{
	socklen_t addrlen = sizeof(struct sockaddr_in);

	// Accept the incoming connection
	if (-1 == (sock->fd = accept(server->fd, (struct sockaddr*) &sock->addr, &addrlen))) {
		perror("Failed to accept an incoming connection");
		free(sock);
		return 0;
	}
	return 1;
}

/**
 * Connect the socket to a server
 *
 * @param socket The socket to connect
 * @param host   The host to connect to
 * @param port   The port number to connect to
 * @return       1 for success, 0 for failure
 */
int network_connect(socket_t *sock, in_addr_t host, int port)
{
	// Copy the host information
	sock->addr.sin_addr.s_addr = host;
	sock->addr.sin_port        = htons(port);

	// Connect to the server
	if (-1 == connect(sock->fd, (struct sockaddr*) &sock->addr, sizeof(sock->addr))) {
		return 0;
	}

	return 1;
}

/**
 * Receive a message from a socket
 *
 * @param sock    The socket to receive from
 * @param buf     Where to store the received message
 * @param n_bytes The max number of bytes to receive
 * @return        The number of bytes read
 */
ssize_t network_receive(socket_t *sock, void *buf, size_t n_bytes)
{
	return read(sock->fd, buf, n_bytes);
}

/**
 * Send a message through a socket
 *
 * @param sock    The socket to send through
 * @param buf     The message to send
 * @param n_bytes The number of bytes to send
 * @return        The number of bytes written; -1 for errors
 */
ssize_t network_send(socket_t *sock, void *buf, size_t n_bytes)
{
	ssize_t bytes_written;

	// Send the network message
	if (-1 == (bytes_written = write(sock->fd, buf, n_bytes))) {
		perror("Failed to write network message");
		printf("FD: %d\n", sock->fd);
	}

	return bytes_written;
}

/**
 * Close the given network socket
 *
 * @param sock The socket to close
 */
void network_close(socket_t *sock)
{
	close(sock->fd);
	free(sock);
}

/**
 * Set the socket's flag to either be blocking or non-blocking
 *
 * @param sock     The socket to set
 * @param blocking Indicate if the socket should be blocking
 * @return         1 Upon success; otherwise 0
 */
int network_set_blocking(socket_t *sock, int blocking)
{
	int flags = fcntl(sock->fd, F_GETFL, 0);
	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	return fcntl(sock->fd, F_SETFL, flags) == 0;
}
