# The build command to use
CC = ortecc
INC = -I$(realpath ../build/include)
LIB = -L$(realpath ../build/lib)

# The RTE framework implementation to use
RTE_FRAMEWORK ?= pmix

# Path Information ---------------------------------------------------------------------------------

# The directory to the built files
BUILD_DIR := ./build

# The directory to the source cade
SOURCE_DIR := ./src

# The directory where test files are
TEST_DIR := ./test

# The directory to the object files
OBJ_DIR := $(BUILD_DIR)/obj

# Makefile Targets ---------------------------------------------------------------------------------

all: openshmem test/shmem_test

# OpenSHMEM ----------------------------------------------------------------------------------------

libopenshmem: openshmem
	@mkdir -p $(BUILD_DIR)/lib
	cp $(SOURCE_DIR)/shmem.h $(BUILD_DIR)/include
	$(CC) -shared $(OBJ_DIR)/*.o \
	            $(OBJ_DIR)/rte/$(RTE_FRAMEWORK)/*.o \
	    -o $(BUILD_DIR)/lib/$@.so

openshmem: comm memory network rte src/shmem.c
	@mkdir -p $(BUILD_DIR)/include
	cp $(SOURCE_DIR)/shmem.h $(BUILD_DIR)/include
	$(CC) -fPIC -c src/shmem.c -o build/obj/shmem.o

comm: $(SOURCE_DIR)/comm/*.c
	@mkdir -p $(OBJ_DIR)/$@
	$(CC) -fPIC -c src/$@/comm.c -o build/obj/$@/comm.o
	$(CC) -fPIC -c src/$@/comm_local.c -o build/obj/$@/comm_local.o
	$(CC) -fPIC -c src/$@/comm_remote.c -o build/obj/$@/comm_remote.o
	$(CC) -fPIC -c src/$@/comm_node.c -o build/obj/$@/comm_node.o

job: $(SOURCE_DIR)/comm/*.c
	@mkdir -p $(OBJ_DIR)/$@

memory: $(SOURCE_DIR)/memory/*.c
	@mkdir -p $(OBJ_DIR)/$@
	$(CC) -fPIC -c src/$@/heap.c -o build/obj/$@/heap.o
	$(CC) -fPIC -c src/$@/hashmap.c -o build/obj/$@/hashmap.o
	$(CC) -fPIC -c src/$@/shared_heap.c -o build/obj/$@/shared_heap.o
	$(CC) -fPIC -c src/$@/shared_mem.c -o build/obj/$@/shared_mem.o

network:
	@mkdir -p $(OBJ_DIR)/$@
	$(CC) -fPIC -c src/$@/netutil.c -o build/obj/$@/netutil.o
	$(CC) -fPIC -c src/$@/network.c -o build/obj/$@/network.o

rte:
	@$(MAKE) -C $(SOURCE_DIR)/rte -f Makefile $(RTE_FRAMEWORK)

# Tests --------------------------------------------------------------------------------------------

tests:
	@$(MAKE) -C $(TEST_DIR) -f Makefile

test/%:
	@$(MAKE) -C $(TEST_DIR) -f Makefile $(notdir $@)

clean:
	rm -rf ./build
	@$(MAKE) -C $(TEST_DIR) -f Makefile clean

.PHONY: all rte clean
