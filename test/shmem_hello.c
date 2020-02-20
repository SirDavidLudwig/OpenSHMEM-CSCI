#include <stdio.h>
#include <stdlib.h>
#include "../src/shmem.h"

int main() {
    int pe, size;

    shmem_init();
    pe = shmem_my_pe();
    size = shmem_n_pes();

    printf("Hello World! There are %d PE's and my PE is %d.", size, pe);

    shmem_finalize();
    return 0;
}
