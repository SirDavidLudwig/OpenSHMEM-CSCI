#include <stdio.h>
#include <unistd.h>
#include <shmem.h>
#include <mpi/mpi.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../src/memory/shared_heap.h"

int main()
{
	int my_pe;
	int n_pes;
	char m;
	struct shared_heap_t *heap_obj;
	char *my_heap;
	char *other_heap;

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_pe);
	MPI_Comm_size(MPI_COMM_WORLD, &n_pes);

	if (my_pe == 0) {
		// my_heap = shared_heap_create("dwl2x-shmem-00", 2 << 25);
		heap_obj = shared_heap_create("dwl2x-shmem-00", 2 << 25);
		my_heap = heap_obj->heap->ptr;

		MPI_Barrier(MPI_COMM_WORLD);

		if (-1 != shared_mem_open("dwl2x-shmem-10", 2 << 25, (void**)&other_heap)) {
			printf("%d: Failed to open other heap\n", my_pe);
		}
		other_heap[0] = 'E';
		MPI_Barrier(MPI_COMM_WORLD);
		printf("The value is: %c\n", my_heap[0]);
	} else {
		// my_heap = shared_heap_create("dwl2x-shmem-10", 2 << 25);
		heap_obj = shared_heap_create("dwl2x-shmem-10", 2 << 25);
		my_heap = heap_obj->heap->ptr;

		MPI_Barrier(MPI_COMM_WORLD);

		if (-1 != shared_mem_open("dwl2x-shmem-00", 2 << 25, (void**)&other_heap)) {
			printf("%d: Failed to open other heap\n", my_pe);
		}
		other_heap[0] = 'H';
		MPI_Barrier(MPI_COMM_WORLD);
		printf("The value is: %c\n", my_heap[0]);
	}

	MPI_Finalize();

	return 0;
}
