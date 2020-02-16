#include <stdio.h>

#include "../src/openshmem.h"

int main()
{
	printf("Hello World!\n");
	shmem_init();

	char *a = shmem_malloc(sizeof(char));
	int *b = shmem_malloc(sizeof(int));

	*a = 'a';
	*b = 100 << 8;

	return 0;
}
