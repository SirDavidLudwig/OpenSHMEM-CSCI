#pragma once

#include <pthread.h>

// void work_start();

/**
 * Send the given data to another PE
 *
 * @return The request ID
 */
int work_put(int pe, long dest, const void *src, size_t size);
