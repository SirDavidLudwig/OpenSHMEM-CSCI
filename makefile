test: test/shmem_test.c src/shmem.c src/shared_memory.c
	gcc test/shmem_test.c src/shmem.c src/shared_memory.c -lrt
