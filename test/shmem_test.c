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

	*a = *b = 0;

	shmem_barrier_all();

	if (my_pe == 0) {
		*a = 10;
		shmem_int_put(b, a, 1, 1);
		while (*b == 0);
		printf("The result is %d\n", *b);
		shmem_getmem(a, b, 1, 1);
		printf("And the get value is %d\n", *a);

	} else {
		while(*b == 0);
		printf("The new value is %d\n", *b);
		*a = *b*2;
		*b = 100;
		shmem_int_put(b, a, 1, 0);
	}

	shmem_finalize();

	return 0;
}
