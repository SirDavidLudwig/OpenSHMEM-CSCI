#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../memory/shared_mem.h"

/**
 * How shared memory is prefixed
 */
#define SHARED_MEMORY_PREFIX "dwl2x"

void comm_node_init(int my_local_pe, int n_local_pes, void **region);

// void comm_node_finalize();
