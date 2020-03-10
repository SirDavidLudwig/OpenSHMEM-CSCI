#include <stdio.h>
#include <unistd.h>
#include <shmem.h>

#include "../src/rte/rte.h"
#include "../src/network/netutil.h"
#include "../src/network/network.h"

int main()
{
	shmem_init();

	char hostname[256];
	int my_pe = shmem_my_pe();
	int n_pes = shmem_n_pes();

	rte_hostname(hostname, NULL);

	printf("%s: My PE is %d, and there are %d PEs\n", hostname, my_pe, n_pes);

	socket_t *server;
	socket_t *sock;
	if (my_pe == 0) {
		server = network_create_server(12345, 5);
	}

	shmem_barrier_all();

	if (my_pe == 0) {
		sleep(5);
		sock = network_accept(server);
		shmem_barrier_all();
		// network_close(sock);
		network_close(server);
	} else {
		in_addr_t host = netutil_resolve_host("gameframe");
		sock = network_create_socket();
		network_connect(sock, host, 12345);
		printf("Connected!\n");
		shmem_barrier_all();
		network_close(sock);
	}

	shmem_finalize();

	return 0;
}
