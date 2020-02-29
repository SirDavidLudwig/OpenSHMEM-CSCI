# The build command to use
CC = mpicc
INC = -I$(realpath ../build/include)
LIB = -L$(realpath ../build/lib)

# Path to the OpenSHMEM library binary
LIB_DIR := /home/david/Development/C/OpenSHMEM-CSCI/build/lib

# Where to build the test files
BUILD_DIR := ./build

# Locate all test files available
TEST_NAMES := $(patsubst %.c,%,$(wildcard *.c))

all: $(TEST_NAMES)

$(TEST_NAMES):
	@mkdir -p $(BUILD_DIR)
	$(CC) $@.c $(INC) $(LIB) -lopenshmem -o $(BUILD_DIR)/$@
