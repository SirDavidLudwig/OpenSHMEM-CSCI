#include "network.h"

pthread_t thread_id;
struct comm_args
{
	int pe;
};

/**
 * The main communication thread
 */
void *comm_thread(void *argptr)
{
	int sockfd;
	struct comm_args *args = (struct comm_args*)argptr;
	struct sockaddr_in server_address;
	int opt = 1;
	int addrlen = sizeof(struct sockaddr_in);
	printf("Pthread port number is %d\n", args->pe);

	return 0;
}

// Interface Functions -----------------------------------------------------------------------------

void comm_init(int pe)
{
	struct comm_args *args = malloc(sizeof(struct comm_args));
	args->pe = pe;
	pthread_create(&thread_id, NULL, comm_thread, args);
}

void comm_finalize()
{
	pthread_join(thread_id, NULL);
}
