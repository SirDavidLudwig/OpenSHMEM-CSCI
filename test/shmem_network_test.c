#include <stdio.h>
#include <unistd.h>
#include <shmem.h>

#include "../src/network/netutil.h"
#include "../src/network/network.h"

int main()
{
	char m;

	shmem_init();

	int my_pe = shmem_my_pe();
	int n_pes = shmem_n_pes();

	printf("My PE is %d, and there are %d PEs\n", my_pe, n_pes);

	int *a = shmem_malloc(sizeof(int));
	int *b = shmem_malloc(sizeof(int));

	*a = *b = 25;

	shmem_barrier_all();

	if (my_pe == 0) {
		*a = 50;
		shmem_putmem(b, a, sizeof(int), 1);
	} else {
		while (*b == 25);
		printf("B is now %d!\n", *b);
	}

	shmem_finalize();

	return 0;
}
