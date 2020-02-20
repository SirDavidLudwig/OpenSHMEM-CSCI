#pragma once

#include <pthread.h>

/**
 * Create and initialize the worker thread
 */
void worker_init();

/**
 * Clean up and detsroy the worker thread
 */
void worker_finalize();
