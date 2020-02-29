# Ooutput Directory
OUT_DIR = ../../build/obj/rte

# All available frameworks implementations
RTE_FRAMEWORKS := $(sort $(patsubst %/,%,$(dir $(wildcard */rte.c))))

# Make all RTE implementations
all: $(RTE_FRAMEWORKS)

# Implementation Building --------------------------------------------------------------------------

# MPI
mpi/%.o: mpi/%.c
	mpicc -fPIC -c $< -o $(OUT_DIR)/$@

# Auto-magic stuff ---------------------------------------------------------------------------------

$(RTE_FRAMEWORKS): $(wildcard $@/*.c)
	@mkdir -p $(OUT_DIR)/$@
	$(MAKE) $(patsubst %.c,%.o,$(wildcard $@/*.c))

.PHONY: all $(RTE_FRAMEWORKS)
