#include "hashmap.h"

/**
 * Calculate the hash for the given heap and key
 */
#define HASH_FUNC(a,b) (int)((long)a % b)

/**
 * Get the node from the given hashmap and key
 */
#define GET_NODE(a, b) a->buckets[HASH_FUNC(b, a->n_buckets)]

/**
 * Create and initialize a hashmap object
 *
 * @param n_buckets The number of buckets to use in the hashmap
 * @return          The hashmap object
 */
struct hashmap_t* hashmap_init(int n_buckets)
{
	struct hashmap_t *hashmap;

	// Create the hashmap
	hashmap = malloc(sizeof(hashmap) + n_buckets*sizeof(struct block_t*));
	hashmap->n_buckets = n_buckets;
	memset(hashmap->buckets, 0, n_buckets*sizeof(struct hashmap_node_t*));

	return hashmap;
}

/**
 * Free a hashmap from memory
 *
 * @param hashmap The hashmap to free
 */
void hashmap_free(struct hashmap_t *hashmap)
{
	struct hashmap_node_t *node, *next;
	int i;

	// Remove the hashmap nodes
	for (i = 0; i < hashmap->n_buckets; i++) {
		for (node = hashmap->buckets[i]; node != NULL; node = next) {
			next = node->next;
			free(node);
		}
	}

	// Free the hashmap itself
	free(hashmap);
}

/**
 * Find a value in the hashmap from the given key
 *
 * @param hashmap The hashmap to search
 * @param key     The key to lookup
 * @return        The address to the
 */
void* hashmap_get(struct hashmap_t *hashmap, void *key)
{
	struct hashmap_node_t *node;

	for (node = GET_NODE(hashmap, key); node != NULL; node = node->next) {
		if (key == node->value->next->ptr) {
			return node->value;
		}
	}

	return NULL;
}

/**
 * Set an existing value in the hashmap
 *
 * @param hashmap The hashmap to work with
 * @param key     The key to lookup
 * @param value   The value to set
 */
void hashmap_set(struct hashmap_t *hashmap, void *key, struct block_t *value)
{
	struct hashmap_node_t *node;
	int index;

	// Calculate the index and locate the node index
	index = HASH_FUNC(key, hashmap->n_buckets);

	// Update the existing node if it exsits
	for (node = hashmap->buckets[index]; node != NULL; node = node->next) {
		if (key == node->value->next->ptr) {
			node->value = value;
			return;
		}
	}

	// Create the node and insert it at the beginning of the bucket
	node = malloc(sizeof(struct hashmap_node_t));
	node->value = value;
	node->next = hashmap->buckets[index];
	hashmap->buckets[index] = node;
}

/**
 * Remove a value from the hashmap
 *
 * @param hashmap The hashmap to work with
 * @param key     The key to lookup
 * @return        The value that was removed
 */
struct block_t *hashmap_remove(struct hashmap_t *hashmap, void *key)
{
	struct block_t *result;
	struct hashmap_node_t **node, *found;
	int index;

	// Calculate the index and locate the node index
	index = HASH_FUNC(key, hashmap->n_buckets);

	// Return if no node exists
	if (hashmap->buckets[index] == NULL) {
		return NULL;
	}

	// Find the value and remove it
	for (node = &hashmap->buckets[index]; *node != NULL; node = &((**node).next)) {
		if (key == (*node)->value->ptr) {
			found = *node;
			result = found->value;
			*node = (*node)->next;
			free(found);
			return result;
		}
	}

	// No node with the given key found
	return NULL;
}
