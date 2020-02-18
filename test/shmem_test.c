#include <stdio.h>

#include "../src/shmem.h"

int main()
{
	printf("Hello World!\n");
	shmem_init();

	char *a = shmem_malloc(sizeof(char));
	int *b = shmem_malloc(sizeof(int));

	*a = 'a';
	*b = 100;

	printf("%d %d\n", *a, *b);

	shmem_finalize();

	return 0;
}
