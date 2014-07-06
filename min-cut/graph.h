#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"

/* Currently going to assume the key is unique for every virtex */

// TODO: Use another list to hold merged nodes!
typedef struct {
  void* key;
  node_t* edge_list;  /* edges for which this node is a head */
  node_t* merged_vertices; /* All previously merged nodes */
} vertex_t;

/* Go ahead and have a key for the edge, too.  May be useful in the future */
typedef struct {
  int key;
  vertex_t* head_vertex;
  vertex_t* tail_vertex;
} edge_t;

typedef struct {
  vertex_t* vertices;
  edge_t* edges;
  /* Number of valid vertices; incontiguous in array */
  long long number_of_vertices;
  /* Number of previously used slots */
  long long vertex_buffer_count;
  long long number_of_edges;
  long long vertice_buffer_size;
  long long edge_buffer_size;
  int is_unidirectional;
  /* Compare function used to find vertices */
  /* Return 1 if equal, 0 otherwise. */
  int (*compare)(void *lhs, void *rhs);

  /* Prints key with no newlines to appropriate buffer */
  void (*print)(FILE *buffer, void *key);

} graph_t;

/* Returns 0 on success */
int
initialize_graph(graph_t *graph, long long vertex_buffer_size,
                 long long edge_buffer_size, int is_unidirectional,
                 int (*compare)(void *lhs, void *rhs),
                 void (*print)(FILE *buffer, void *key));

/* Create vertex.  Expects all neighbors to exist */
/* Returns pointer to new vertex on success; NULL otherwise */
vertex_t*
add_vertex(graph_t *graph, void *key, void **neighbors, int number_of_neighbors);

/* Add in a set of vertices all vertices */
/* Neighbors must be in list provided */
int
add_vertices(graph_t *graph, void **keys, int number_of_keys,
             void ***neighbors, int *number_of_neighbors);

/* Returns pointer to vertex if found; NULL otherwise */
vertex_t*
find_vertex_by_key(graph_t *graph, void *key);

/******************************************************
 * Printing functions
 *****************************************************/

/*  Prints the adjacency list in form:
 *  Node 1 : neighbor 1 -> ... -> neighbor m
 *  ...
 *  Node N : neighbor 1 -> ... -> neighbor k
 */
void
print_graph_vertices(graph_t *graph);

/*  Prints the adjacency list in form:
 *  [node i] -> [node j] where i and j are neighbors
 */
void
print_graph_edges(graph_t *graph);

/* Prints both vertices and edges */
/* Flags : bit 0 = print vertices
           bit 1 = print edges
           bit 2 = print merged nodes
 */ 
void
print_graph(graph_t *graph, char flags);

/* Prints list of merged edges */
void
print_merged_edges(graph_t *graph);

/****************************************************/

/* Merge Nth and Mth nodes where edge_n connects N and M.
 * -- Does remove self referencing loops after merge.
 * -- Calls merge_vertices_of_edge to do heavy lifting.
 * Returns pointer to new super vertex.
 */
vertex_t* merge_edge_and_remove_loops(graph_t *graph, int edge_n);


/* Merge edges
 * - Combine two nodes connected by a single edge.
 * - Assume we merge tail into head.
 * 1. Find all edges of the tail and point to the head in the list instead.
 *    -- Assumes that both nodes have pointers to each other.
 * 2. Traverse through tail of all edges and change pointers to tail to new head.
 * 3. Merge edge list of tail into merge list of head
 * 4. **Does NOT remove self-referring nodes.
 * Returns pointer to new super node.
 */
 vertex_t*
 merge_vertices_of_edge(graph_t *graph, edge_t *edge);

/* Removes self-referring edges in a edge list.
 * Does so in O(k) time, where k is number of edges of the node, so
 * essentially O(1) time for N edges overall.
 */
void
remove_loops_of_vertex(graph_t *graph, vertex_t *v);

#endif
