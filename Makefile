# Variables ----------------------------------------------------------------------------------------

# The RTE framework implementation to use
RTE_FRAMEWORK ?= mpi

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

all: openshmem test/shmem_heap

# OpenSHMEM ----------------------------------------------------------------------------------------

libopenshmem: openshmem
	@mkdir -p $(BUILD_DIR)/lib
	cp $(SOURCE_DIR)/shmem.h $(BUILD_DIR)/include
	gcc -shared $(OBJ_DIR)/*.o \
	            $(OBJ_DIR)/rte/$(RTE_FRAMEWORK)/*.o \
	    -o $(BUILD_DIR)/lib/$@.so


openshmem: comm memory network rte src/comm.c src/shmem.c
	@mkdir -p $(BUILD_DIR)/include
	cp $(SOURCE_DIR)/shmem.h $(BUILD_DIR)/include
	gcc -fPIC -c src/comm.c -o build/obj/comm.o
	gcc -fPIC -c src/shmem.c -o build/obj/shmem.o

comm:

memory: $(SOURCE_DIR)/memory/*.c
	@mkdir -p $(OBJ_DIR)/memory
	gcc -fPIC -c src/memory/heap.c -o build/obj/memory/heap.o
	gcc -fPIC -c src/memory/hashmap.c -o build/obj/memory/hashmap.o
	gcc -fPIC -c src/memory/shared_mem.c -o build/obj/memory/shared_mem.o

network:
	@mkdir -p $(OBJ_DIR)/network

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
