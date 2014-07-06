#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>
#include <assert.h>

struct node_t {
  void *key;
  struct node_t *next;
};

typedef struct node_t node_t;

/* Insert node at head */
void insert_at_head(node_t **head, void *key);

/* Insert node at end */
void insert_at_tail(node_t **head, void *key);

/* Remove node using poiner to a specific node 
 * Returns key if found, NULL otherwise.
 */
void* delete_node(node_t **head, node_t *node);

/* Remove node using key and comparison function 
 * Will remove any node in list that returns 1 from
 * the supplied comparison function.
 * Returns number of nodes deleted.
 */
int delete_nodes_by_key(node_t **head, void *key, 
                        int (*compare)(void *node_key, void *search_key));

/* Appends list B to list A
 * Returns -1 on error and restores both lists
 */
int append(node_t *head_A, node_t *head_B);

/* Get next item of array */
node_t* next_item(node_t *node);

/* Search for a given key; returns NULL if nothing found. */
node_t* find_key(node_t *head, void *key, int (*compare)(void *lhs, void *rhs));

/* Returns an array of keys 
 * If size == NULL or <=0, start w/ default size of 16.
 */
void **arrayitize(node_t *head, int *size, int *count);
#endif
