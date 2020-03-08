#include <stdio.h>
#include <unistd.h>
#include <shmem.h>

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
		*a = 10;
		shmem_putmem(b, a, sizeof(char), 1);
		while (*b == 0);
		printf("The result is %d\n", *b);
		shmem_getmem(a, b, sizeof(char), 1);
		printf("And the get value is %d\n", *a);

	} else {
		while(*b == 0);
		*a = *b*2;
		*b = 100;
		shmem_putmem(b, a, sizeof(char), 0);
	}

	shmem_finalize();

	return 0;
}
