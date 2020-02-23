BUILD_DIR  := ./build
OBJ_DIR    := $(BUILD_DIR)/obj
SRC_DIR    := ./src
TEST_DIR   := ./test
SRC_FILES  := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES  := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: shmem_test shmem_hello

shmem_test: $(OBJ_FILES) test/shmem_test.c
	mpicc -o $(BUILD_DIR)/$@ $^ -lrt -lpthread

shmem_hello: $(OBJ_FILES) test/shmem_hello.c
	mpicc -o $(BUILD_DIR)/$@ $^ -lrt -lpthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	mpicc -c -o $@ $<

clean:
	rm -rf build
