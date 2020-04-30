# The build command to use
CC = ortecc
INC = -I$(realpath ../build/include)
LIB = -L$(realpath ../build/lib)

# The RTE framework to use
RTE_FRAMEWORK ?= pmix

# Path to the OpenSHMEM library binary
LIB_DIR := /nfshome/dwl2x/OpenSHMEM-CSCI/build/lib

# Where to build the test files
BUILD_DIR := ./build

# Where the built object files are
OBJ_DIR := ../build/obj

# The object files to link with
OBJ_FILES := $(OBJ_DIR)/*.o \
             $(OBJ_DIR)/comm/*.o \
             $(OBJ_DIR)/job/*.o \
             $(OBJ_DIR)/memory/*.o \
			 $(OBJ_DIR)/network/*.o \
			 $(OBJ_DIR)/rte/$(RTE_FRAMEWORK)/*.o

# Locate all test files available
TEST_NAMES := $(patsubst %.c,%,$(wildcard *.c))

all: $(TEST_NAMES)

clean:
	rm -rf build

$(TEST_NAMES):
	@mkdir -p $(BUILD_DIR)
	$(CC) $@.c $(INC) $(LIB) -lopenshmem -o $(BUILD_DIR)/$@ -lrt -lpthread -lpmix -lm
	# $(CC) $@.c $(INC) $(OBJ_FILES) -o $(BUILD_DIR)/$@ -lrt -lpthread -lm -l$(RTE_FRAMEWORK)

.PHONY: all clean
