hello: test/shmem_hello.c src/shmem.c src/shared_memory.c
	mpicc test/shmem_hello.c src/shmem.c src/shared_memory.c -lrt

test: test/shmem_test.c src/shmem.c src/shared_memory.c
	mpicc test/shmem_test.c src/shmem.c src/shared_memory.c -lrt
