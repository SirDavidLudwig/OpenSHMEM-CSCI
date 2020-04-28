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
static socket_t *__sockets_cmd = NULL;
static socket_t *__sockets_data = NULL;

// Communication Thread ----------------------------------------------------------------------------

/**
 * A reference to the communication thread
 */
static pthread_t __comm_thread_id;

/**
 * A lock to ensure thread-safe queue manipulation
 */
static pthread_mutex_t __comm_thread_lock;

/**
 * Indicate if the thread is finished
 */
static char __finished = 0;

/**
 * A queue of information to send
 */
static packet_queue_t *__queue_send;
static packet_queue_t *__queue_send_tail;

/**
 * A record of the request indices mapped by PE
 */
static long *__request_indices;

/**
 * A buffer for received packets
 */
static packet_t *__packet_buf;

/**
 * A thread-safe function to enqueue a network packet
 *
 * @param packet The packet to enqueue
 */
static void __comm_thread_enqueue(int pe_index, packet_t *packet)
{
	pthread_mutex_lock(&__comm_thread_lock);
	if (__queue_send == NULL) {
		__queue_send_tail = __queue_send = malloc(sizeof(packet_queue_t));
	} else {
		__queue_send_tail = __queue_send_tail->next = malloc(sizeof(packet_queue_t));
	}
	__queue_send_tail->packet = packet;
	__queue_send_tail->pe_index = pe_index;
	__queue_send_tail->next = NULL;
	pthread_mutex_unlock(&__comm_thread_lock);
}

/**
 * A thread-safe function to dequeue a network packet
 *
 * @return A pointer to the dequeued packet if it exists; otherwise NULL
 */
static packet_t* __comm_thread_dequeue(int *pe_index)
{
	packet_queue_t *queued_packet;
	packet_t *result = NULL;

	pthread_mutex_lock(&__comm_thread_lock);
	if (NULL != (queued_packet = __queue_send)) {
		printf("Dequeued a packet\n");
		result = __queue_send->packet;
		*pe_index = queued_packet->pe_index;
		__queue_send = queued_packet->next;
		free(queued_packet);
	}
	pthread_mutex_unlock(&__comm_thread_lock);
	return result;
}

static void __comm_thread_receive()
{
	int i;
	ssize_t bytes_read;
	size_t packet_size = sizeof(packet_t);
	void *data_buf;

	for (i = 0; i < __n_remote_pes; i++) {
		if (-1 != (bytes_read = network_receive(&__sockets_cmd[i], __packet_buf, packet_size))) {
			if (bytes_read != packet_size) {
				printf("%d: ERROR: Bad packet received. Expected %ld, got %ld\n", __my_pe, packet_size, bytes_read);
				continue;
			}
			data_buf = malloc(__packet_buf->size);
			bytes_read = network_receive(&__sockets_cmd[i], data_buf, __packet_buf->size);
			if (bytes_read != __packet_buf->size) {
				printf("%d: ERROR: Bad packet received. Expected %ld, got %ld\n", __my_pe, __packet_buf->size, bytes_read);
			}
		}
	}
}

static void __comm_thread_send(socket_t *socket_group)
{
	int pe_index;
	ssize_t bytes_written;
	packet_t *packet;

	while (__queue_send != NULL) {
		packet = __comm_thread_dequeue(&pe_index);
		packet->request_index = ++__request_indices[pe_index];
		bytes_written = network_send(&socket_group[pe_index], packet, sizeof(packet_t) + packet->size);
		printf("Sent a packet: %ld\n", bytes_written);
		free(packet);
	}
}

/**
 * The communication thread function
 */
static void* __comm_thread()
{
	while (!__finished) {
		__comm_thread_receive();
		__comm_thread_send(__sockets_cmd);
	}
}

// Private Member Functions ------------------------------------------------------------------------

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
	__finished = 1;

	pthread_join(__comm_thread_id, NULL);
	pthread_mutex_destroy(&__comm_thread_lock);
	free(__request_indices);
	free(__packet_buf);
	__comm_remote_close_servers();
}

/**
 * Wire up remote processes
 */
void comm_remote_wireup()
{
	int levels = ceil(log2(__n_pes));

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

	for (int i = 0; i < __n_remote_pes; i++) {
		if (0 == network_set_blocking(&__sockets_cmd[i], 0)) {
			printf("%d: ERROR: Failed to set sockets as non-blocking\n", __my_pe);
		}
	}
}

/**
 * Start the remote communication thread
 */
void comm_remote_start()
{
	__queue_send = NULL;
	__queue_send_tail = NULL;
	__finished = 0;
	__packet_buf = malloc(sizeof(packet_t));
	__request_indices = malloc(__n_remote_pes * sizeof(long));
	memset(__request_indices, 0, __n_remote_pes * sizeof(long));
	pthread_mutex_init(&__comm_thread_lock, NULL);
	pthread_create(&__comm_thread_id, NULL, __comm_thread, NULL);
}

// Communication Methods ---------------------------------------------------------------------------

/**
 * Get a value from a local PE
 *
 * @param pe   The PE to communicate with
 * @param dest The destination variable to put the result
 * @param src  The source offset position within the heap
 * @param size The number of bytes to send
 */
void comm_remote_get(int pe, void *dest, long src, size_t size)
{

}

/**
 * Put a value into a local PE
 *
 * @param pe   The PE to communicate with
 * @param dest The destination offset position within the heap
 * @param src  The source variable to send
 * @param size The number of bytes to send
 */
void comm_remote_put(int pe, long dest, const void *src, size_t size)
{
	packet_t *packet;

	printf("Performing a put request\n");

	// Create the packet
	packet = malloc(sizeof(packet) + size);
	packet->handler = HANDLER_PUT;
	packet->size = size;
	packet->origin = __my_pe;
	packet->heap_offset = dest;
	memcpy(packet->data, src, size);

	// Enqueue the packet
	__comm_thread_enqueue(comm_node_map(pe)->index, packet);
}
