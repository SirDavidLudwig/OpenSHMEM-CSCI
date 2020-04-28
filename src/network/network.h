#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * A simple container for sockets
 */
typedef struct socket
{
	int fd;                  // The socket's file descriptor
	struct sockaddr_in addr; // The socket's address information
} socket_t;

/**
 * Initialize an existing socket object
 *
 * @param sock The socket to initialize
 * @return     1 for success; otherwise 0
 */
int network_init_socket(socket_t *sock);

/**
 * Create a socket
 *
 * @return The created socket
 */
socket_t* network_create_socket();

/**
 * Create a server socket
 *
 * @param port     The port to bind to
 * @param n_listen The maximum number of connections to listen for
 * @return         The created server socket
 */
socket_t* network_create_server(int port, int n_listen);

/**
 * Accept a connection if available
 *
 * @param server The server to accept from
 * @return       The newly connected socket
 */
socket_t* network_accept(socket_t *server);

/**
 * Accept a connection if available
 *
 * @param server The server to accept from
 * @param sock   The socket to store the result
 * @return       1 for success; otherwise 0
 */
int network_accept_2(socket_t *server, socket_t *sock);

/**
 * Connect the socket to a server
 *
 * @param socket The socket to connect
 * @param host   The host to connect to
 * @param port   The port number to connect to
 * @return       1 for success; 0 for failure
 */
int network_connect(socket_t *sock, in_addr_t host, int port);

/**
 * Receive a message from a socket
 *
 * @param sock    The socket to receive from
 * @param buf     Where to store the received message
 * @param n_bytes The max number of bytes to receive
 * @return        The number
 */
ssize_t network_receive(socket_t *sock, void *buf, size_t n_bytes);

/**
 * Send a message through a socket
 *
 * @param sock    The socket to send through
 * @param buf     The message to send
 * @param n_bytes The number of bytes to send
 * @return        Returns the number of bytes written; -1 for errors
 */
ssize_t network_send(socket_t *sock, void *buf, size_t n_bytes);

/**
 * Close the given network socket
 *
 * @param sock The socket to close
 */
void network_close(socket_t *sock);

/**
 * Set the socket's flag to either be blocking or non-blocking
 *
 * @param sock     The socket to set
 * @param blocking Indicate if the socket should be blocking
 * @return         1 Upon success; otherwise 0
 */
int network_set_blocking(socket_t *sock, int blocking);
