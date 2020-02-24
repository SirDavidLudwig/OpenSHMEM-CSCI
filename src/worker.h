#pragma once

#include <pthread.h>

/**
 * Create and initialize the worker thread
 */
void worker_init();

/**
 * Check if the worker thread is ready
 */
int worker_is_ready();

/**
 * Clean up and detsroy the worker thread
 */
void worker_finalize();
