#pragma once

#include "math.h"
#include "../common.h"
#include "../network/network.h"
#include "comm_node.h"

/**
 * Initialize remote communication
 */
void comm_remote_init(int my_pe, int n_pes, int n_remote_pes);

/**
 * finalize the remote communication layer
 */
void comm_remote_finalize();

/**
 * Wire up remote processes
 */
void comm_remote_wireup();
