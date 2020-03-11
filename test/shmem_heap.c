#include <stdio.h>
#include <unistd.h>
#include "../src/memory/shared_mem.h"
#include "../src/memory/heap.h"

int main()
{
	char m;
	struct heap_t *heap;
	void *region;
	void *addr;

	// Create the shared memory region
	shared_mem_create("dwl2x-mem-test", 1024, &region);

	// Create the heap using the region
	heap = heap_init(region, 1024);

	char *a = heap_malloc(heap, sizeof(char));
	int *b = heap_malloc(heap, sizeof(int));
	char *c = heap_malloc(heap, sizeof(char));
	// char *b = heap_malloc(heap, 2000 * sizeof(char));

	*a = 'h';
	*b = 42;
	*c = 'J';

	if ((long)b - (long)a != sizeof(char) || (long)c - (long)b != sizeof(int)) {
		perror("Heap not spacing correctly");
		return 1;
	}

	// Allocate the rest of the space
	char *d = heap_malloc(heap, 1018);
	if (d == NULL) {
		perror("Failed to fill the rest of the space");
		return 1;
	}

	// Try to allocate something in a full heap
	if (heap_malloc(heap, 1) != NULL) {
		perror("Allocated when no space was available");
		return 1;
	}

	// Free the first byte and malloc again
	addr = a;
	heap_free(heap, a);
	a = heap_malloc(heap, sizeof(char));
	if (a == NULL || addr != a) {
		perror("Failed to allocate a byte after already freeing one");
		return 1;
	}

	// Free an integer located in the middle of the heap somewhere
	addr = b;
	heap_free(heap, b);
	b = heap_malloc(heap, sizeof(int));
	if (b == NULL || addr != b) {
		perror("Failed to allocate a integer after already freeing one");
		return 1;
	}

	printf("Success %p %p %p\n", a, b, c);

	heap_finalize(heap);

	return 0;
}
