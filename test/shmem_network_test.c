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

	char *a = shmem_malloc(sizeof(char));
	char *b = shmem_malloc(sizeof(char));

	*a = *b = 0;

	shmem_barrier_all();

	if (my_pe == 0) {
		*a = 50;
		// shmem_putmem(b, a, sizeof(int), 1);
	} else {

	}

	shmem_finalize();

	return 0;
}
