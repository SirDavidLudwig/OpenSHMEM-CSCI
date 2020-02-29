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

	// char *a = shmem_malloc(sizeof(char));
	// char *b = shmem_malloc(sizeof(char));
	// char *c = shmem_malloc(sizeof(char));

	// *a = -1;
	// *b = -1;

	// shmem_barrier_all();

	// if (my_pe == 0) {
	// 	m = 'X';
	// 	*c = '7';

	// 	// Wait for PE 1 to set a
	// 	while (*a == -1);
	// 	printf("The value of a is %c\n", *a);

	// 	// Put the value of m to b on PE 1
	// 	shmem_put(b, &m, 1, 1);

	// 	// Get the value of c on PE 1
	// 	shmem_get(&m, c, 1, 1);
	// } else {
	// 	m = 'Z';
	// 	*c = '3';

	// 	// Put the value of m to a on PE 0
	// 	shmem_put(a, &m, 1, 0);

	// 	// Wait for PE 0 to set b
	// 	while (*b == -1);
	// 	printf("The value of b is %c\n", *b);

	// 	// Get the value of c on PE 0
	// 	shmem_get(&m, c, 1, 0);
	// }

	// printf("%d: The value of c is %c\n", my_pe, m);

	shmem_finalize();

	return 0;
}
