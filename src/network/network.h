#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sched.h>
#include <fcntl.h>

/**
 * A simple container for sockets
 */
struct socket_t
{
	int fd;                  // The socket's file descriptor
	struct sockaddr_in addr; // The socket's address information
};

struct packet_t
{

};


/**
 * Create a socket
 */
struct socket_t* create_socket();

/**
 * Create a server socket
 */
struct socket_t* create_server_socket(int port);
