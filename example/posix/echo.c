#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    size_t SIZE = (1<<12);
    int fd;    

    char name[255];
    char * shared;

    sprintf(name, "/%d-shmem-%d", my_pe);    

    // create shared object (with a name)
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Shared memory failed\n");
        return -1;
    }

    // allocate the size of the object
    ftruncate(fd, SIZE);

    shared = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap: ");
        return -1;
    }

    memset(shared, 0, SIZE);    

    while (1) {
        if (shared[0] == 0) {
            continue;
        }

        printf("Message from client: %s\n", (shared+1));
        shared[0] = 0;
    }

    munmap(shared, SIZE);
    close(fd);
    return 0;
}
