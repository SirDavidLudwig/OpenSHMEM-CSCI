#include <stdio.h>
#include <unistd.h>

#include "../src/shmem.h"

int main()
{
	printf("Hello World!\n");
	shmem_init();

	int my_pe = shmem_my_pe();
	int n_pes = shmem_n_pes();

	char *a = shmem_malloc(sizeof(char));
	char *b = shmem_malloc(sizeof(char));

	shmem_barrier_all();

	if (my_pe == 0) {
		char m = 'X';
		sleep(1);
		printf("The value of a is %c\n", *a);
		shmem_put(b, &m, 1, 1);
	} else {
		char m = 'Z';
		shmem_put(a, &m, 1, 0);
		*b = 0;
		while (*b == 0) {
			continue;
		}
		printf("The value of b is %c\n", *b);
	}

	shmem_finalize();

	return 0;
}
