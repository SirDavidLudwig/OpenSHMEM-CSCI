#pragma once

/**
 * Initialize remote communication
 */
void comm_remote_init(int my_pe, int n_pes);

/**
 * Wire up remote processes
 */
void comm_remote_wireup();
