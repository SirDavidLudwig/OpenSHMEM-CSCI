#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char ** argv)
{
    size_t SIZE = (1<<12);
    int fd;    

    char name[255];
    char * shared;

    sprintf(name, "/hello");    

    // create shared object (with a name)
    fd = shm_open(name, O_RDWR, 0666);
    if (fd == -1) {
        perror("Shared memory failed\n");
        return -1;
    }

    shared = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap: ");
        return -1;
    }

    sprintf(shared+1, "%s", argv[1]);
    shared[0] = 1;

    munmap(shared, SIZE);
    close(fd);
    return 0;
}
