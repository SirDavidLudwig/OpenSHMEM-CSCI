test: test/shmem_test.c src/openshmem.c src/shmem.c
	gcc test/shmem_test.c src/openshmem.c src/shmem.c -lrt
