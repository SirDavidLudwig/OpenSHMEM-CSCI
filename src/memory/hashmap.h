#pragma once

#include <stdlib.h>

#include "heap.h"

/**
 * A bucket node to store hashmap values
 */
struct hashmap_node_t
{
	struct block_t *value;
	struct hashmap_node_t *next;
};

/**
 * The hashmap object
 */
struct hashmap_t
{
	int n_buckets;
	struct hashmap_node_t *buckets[];
};

/**
 * Create and initialize a hashmap object
 *
 * @param n_buckets The number of buckets to use in the hashmap
 * @return          The hashmap object
 */
struct hashmap_t* hashmap_init(int n_buckets);

/**
 * Free a hashmap from memory
 *
 * @param hashmap The hashmap to free
 */
void hashmap_free(struct hashmap_t *hashmap);

	/**
 * Find a value in the hashmap from the given key
 *
 * @param hashmap The hashmap to search
 * @param key     The key to lookup
 * @return        The value at the key, or NULL if not found
 */
	void *hashmap_get(struct hashmap_t *hashmap, void *key);

/**
 * Set an existing value in the hashmap
 *
 * @param hashmap The hashmap to work with
 * @param key     The key to lookup
 * @param value   The value to set
 */
void hashmap_set(struct hashmap_t *hashmap, void *key, struct block_t *value);

/**
 * Remove a value from the hashmap
 *
 * @param hashmap The hashmap to work with
 * @param key     The key to lookup
 * @return        The value that was removed
 */
struct block_t* hashmap_remove(struct hashmap_t *hashmap, void *key);
