#include <stdio.h>
#include <stdlib.h>
#include "../src/shmem.h"

int main(int argc, char *argv[]) {
    int pe, size;

    shmem_init(argc, argv);
    pe = shmem_my_pe();
    size = shmem_n_pes();

    printf("Hello World! There are %d PE's and my PE is %d.", size, pe);

    shmem_finalize();
    return 0;
}
