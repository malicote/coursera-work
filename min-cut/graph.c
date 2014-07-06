#include "graph.h"

//TODO: Clean up error signals.
//TODO: Make generic with void*

/* Verify element pointer is valid and in range */
int _verify_address_in_range(unsigned long long starting_address,
                            unsigned long long size_of_array_in_bytes,
                            unsigned long long element_address)
{
  return (element_address >= starting_address &&
          element_address < starting_address + size_of_array_in_bytes);
}

/* Given a list node, return the neighbor */
vertex_t*
_get_neighbor_vertex(node_t* list_node) {
  vertex_t *tail_vertex = NULL;

  if (list_node) {
    edge_t *neighbor_edge = (edge_t *)list_node->key;
    if (neighbor_edge) {
      tail_vertex = neighbor_edge->tail_vertex;
    }
  }
  return tail_vertex;
}

vertex_t*
find_vertex_by_key(graph_t *graph, void *key) {
  int count = 0;
  vertex_t *vertice = NULL;
  int i;

  if(!graph) {
    return NULL;
  }

  /* This always needs to be true */
  assert(graph->compare);

  vertice = graph->vertices;
  count = graph->vertex_buffer_count;

  for (i = 0; i < count; i++) {
    if (graph->compare(vertice[i].key, key)) {
      return &vertice[i];
    }
  }
  return NULL;
}

/* Adds the new vertex to vertex list.
 * Does not attach neighbors.
 * Purpose is to dynamically control vector size.
 * Returns pointer to newly added vertex.
 */
static
vertex_t* _insert_vertex(graph_t *graph, void *key) {
  int size = graph->vertice_buffer_size;
  int count = graph->vertex_buffer_count;
  vertex_t* buff = graph->vertices;

  if (!graph) {
    return NULL;
  }

  /* Bounds check */
  if (count == size - 1) {
    printf("ERROR: Out of bounds in graph adding vertex [size=%d]\n",
           count);
    assert(0);
  }

  /* Add new vertex as requested */
  buff[count].key = key;
  
  /* Clear other variables in case of re-sizing of list. */
  buff[count].edge_list = NULL;  
  buff[count].merged_vertices = NULL;
  
  graph->vertice_buffer_size = size;
  graph->vertex_buffer_count = count + 1;
  graph->number_of_vertices = count + 1;
  return &buff[count];
}

/* Creates a new edge in edge list.
 * Returns pointer to the new edge, NULL on error.
 */
static edge_t*
_insert_edge(graph_t *graph, vertex_t *head, vertex_t *tail) {
  int size = graph->edge_buffer_size;
  int count = graph->number_of_edges;
  edge_t *buff = graph->edges;

  if (!graph || !head || !tail) {
    return NULL;
  }

  /* Bounds check */
  if (count == size - 1) {
    printf("ERROR: Out of bounds in graph adding vertex [size=%d]\n",
           count);
    assert(0);
  }

  /* Add new edge as requested. */
  buff[count].head_vertex = head;
  buff[count].tail_vertex = tail;

  graph->edge_buffer_size = size;
  graph->number_of_edges = count + 1;
  return &buff[count];
}


/* Creates all edges for the new vertex.  Returns -1 on error. */
int
_create_edges(graph_t *graph, vertex_t *new_vertex,
              void **neighbors, int number_of_neighbors)
{
  int error_occurred = 0;

  if(!graph || !new_vertex || !neighbors)
    return -1;

  edge_t *new_edge = NULL;
  int i = 0;

  for (i = 0; i < number_of_neighbors; i++) {
    // Find the vertex to get pointer for tail

    vertex_t *tail = find_vertex_by_key(graph, neighbors[i]);
    if (!tail) {
      fprintf(stderr, "Couldn't find key:");
      graph->print(stderr, neighbors[i]);
      fprintf(stderr, "\n");
      error_occurred = -1;
      continue;
    }

    // Create an edge entry with new head and tail
    new_edge = _insert_edge(graph, new_vertex, tail);
    if(!new_edge) {
      fprintf(stderr, "Couldn't make edge for [");
      graph->print(stderr, new_vertex->key);
      fprintf(stderr, "]->[");
      graph->print(stderr, tail->key);
      fprintf(stderr, "]\n");

      error_occurred = -1;
      continue;
    }
    // Add entry in edge list of head vertex
   insert_at_tail(&new_vertex->edge_list, new_edge);
  }
  return 0;
}
/* Adds a new vertex to the graph.
 * Input: (graph g, new vertex, list of edges)
 * Attaches edges
 * Returns pointer to new vertex, NULL if error.
 */
vertex_t*
add_vertex(graph_t *graph, void *key, void **neighbors, int number_of_neighbors) {
  int ret = 0;

  /* Create the vertex */
  vertex_t *new_vertex = _insert_vertex(graph, key);
  if(!new_vertex) {
    fprintf(stderr, "Error creating new vertex [");
    graph->print(stderr, key);
    fprintf(stderr, "]!\n");
    return NULL;
  }

  ret = _create_edges(graph, new_vertex, neighbors, number_of_neighbors);

  if (ret < 0) {
      fprintf(stderr, "Error creating edges.\n");
  }
  return new_vertex;
}


/* Add in a set of vertices all vertices
 * Neighbors must be in list provided
 * "neighbors" : each entery is an array of neighbors for vertex k
 * "number_of_numbers" : corresponding number of neighbors for vertex k
 */
int
add_vertices(graph_t *graph, void **keys, int number_of_keys,
             void ***neighbors, int *number_of_neighbors)
{
  if (!graph || !keys || !neighbors || !number_of_neighbors) {
    return 0;
  }

  /* First we insert all vertices */
  int v = 0;
  for(v = 0; v < number_of_keys; v++) {
    vertex_t *nv = _insert_vertex(graph, keys[v]);
    assert(nv);
  }
  /* Then we create the edges */
  for(v = 0; v < number_of_keys; v++) {
    int ret  = _create_edges(graph, &graph->vertices[v],
                  neighbors[v], number_of_neighbors[v]);
    if(ret < 0) {
      printf("FAILED TO CREATE NEW EDGE!\n");
      assert(0);
    }
  }
  return 0;
}

int initialize_graph(graph_t* graph, 
                     long long vertice_buffer_size,
                     long long edge_buffer_size,
                     int is_unidirectional,
                     int (*compare)(void *lhs, void *rhs),
                     void (*print)(FILE *buffer, void *key))
{
  graph->vertices = calloc(vertice_buffer_size,  sizeof(vertex_t));
  if (!graph->vertices) {
    return -1;
  }

  graph->edges = calloc(edge_buffer_size, sizeof(edge_t));
  if (!graph->edges) {
    free(graph->vertices);
    graph->vertices = NULL;
    return -1;
  }

  if (compare) {
    graph->compare = compare;
  }

  if (print) {
    graph->print = print;
  }

  graph->vertex_buffer_count = 0;
  graph->number_of_vertices = 0;
  graph->number_of_edges = 0;
  graph->vertice_buffer_size = vertice_buffer_size;
  graph->edge_buffer_size = edge_buffer_size;
  graph->is_unidirectional = is_unidirectional;
  return 0;
}

void print_graph_vertices(graph_t *graph) {
  int v = 0;
  node_t *list_edge;

  if(!graph)
    printf("<NULL>");

  for (v = 0; v < graph->vertex_buffer_count; v++) {

    printf("[");
    graph->print(stdout, graph->vertices[v].key);
    printf("] : ");

    list_edge = graph->vertices[v].edge_list;
    while(list_edge) {
      vertex_t *neighbor = _get_neighbor_vertex(list_edge);

      printf("[");

      if(neighbor)
        graph->print(stdout, neighbor->key);
      else
        printf("<NULL>");

      printf("]->");

      list_edge = list_edge->next;
    }

    printf("[NULL]\n");
  }
}

void print_graph_edges(graph_t *graph) {
  int e = 0;

  if(!graph)
    printf("<NULL>");

  for (e = 0; e < graph->number_of_edges; e++) {

    vertex_t *v = (vertex_t *)graph->edges[e].head_vertex;

    printf("[");
    if(v)
      graph->print(stdout, v->key);
    else
      printf("NULL VERTEX IN EDGE");
    printf("]->");

    v = (vertex_t *)graph->edges[e].tail_vertex;

    printf("[");
    if(v)
      graph->print(stdout, v->key);
    else
      printf("NULL VERTEX IN EDGE");
    printf("]\n");

  }
}


void print_graph_vertice_merge_history(graph_t *graph) {
  int v = 0;

  if(!graph)
    printf("<NULL>");

  for (v = 0; v < graph->vertex_buffer_count; v++) {

    void *key = graph->vertices[v].key;
    if(!key)
      continue;

    printf("[");
    graph->print(stdout,key);
    printf("] : {");

    node_t* merge_history_list = graph->vertices[v].merged_vertices;
    while(merge_history_list) {
      if(merge_history_list->key)
        graph->print(stdout, merge_history_list->key);
      else
        printf("<NULL>");

      printf(", ");
      merge_history_list = merge_history_list->next;
    }
      printf("-}\n");
  }
}

void print_graph(graph_t *graph, char flags) {
  if(flags & 0x1) {
    printf("Graph vertices:\n");
    print_graph_vertices(graph);
    printf("\n------------------------\n");
  }
  if(flags & 0x2) {
    printf("Graph edges\n");
    print_graph_edges(graph);
  }
  if(flags & 0x4) {
    printf("\n------------------------\n");
    printf("Merge history:\n");
    print_graph_vertice_merge_history(graph);
  }
}

/* Given an edge list node, will point you to neighbors edges */
node_t*
_get_neighbors_edges(node_t* edge_list_node) {
  vertex_t *neighbor_vertex = _get_neighbor_vertex(edge_list_node);
  node_t *neighbor_edges = NULL;

  if(neighbor_vertex)
    neighbor_edges = neighbor_vertex->edge_list;

  return neighbor_edges;
}

vertex_t*
_change_tail_of_edge(edge_t* edge, vertex_t *new_vertex) {
  if(!edge)
    return NULL;
  edge->tail_vertex = new_vertex;
  return edge->tail_vertex;
}

/* Change the tail pointer of an edge
 * Returns pointer to vertex data structure, NULL otherwise.
 */
vertex_t*
_change_head_of_edge(edge_t* edge, vertex_t *new_vertex) {
  if(!edge)
    return NULL;
  edge->head_vertex = new_vertex;
  return edge->head_vertex;
}

/* Change the tail pointer of an edge node
 * Returns pointer to edge data structure, NULL otherwise.
 */
edge_t*
_change_tail_of_edge_node(node_t* edge_list_node, vertex_t *new_vertex) {
  if(!edge_list_node)
    return NULL;

  edge_t *neighbor_edge = (edge_t *)edge_list_node->key;
  if(neighbor_edge)
    neighbor_edge->tail_vertex = new_vertex;

  return neighbor_edge;
}

/* Change the tail pointer of an edge node
 * Returns pointer to edge data structure, NULL otherwise.
 */
edge_t*
_change_head_of_edge_node(node_t* edge_list_node, vertex_t *new_vertex) {
  if(!edge_list_node)
    return NULL;

  edge_t *neighbor_edge = (edge_t *)edge_list_node->key;
  if(neighbor_edge)
    neighbor_edge->head_vertex = new_vertex;

  return neighbor_edge;
}


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
 merge_vertices_of_edge(graph_t *graph, edge_t *edge) {
   if (!graph || !edge)
     return NULL;

  /* Make all references to tail node pointer to head node */
  /* Assuming all references to tail node can be reached by edges from tail */
  vertex_t *head_vertex = edge->head_vertex;
  vertex_t *tail_vertex = edge->tail_vertex;
  node_t *tail_edges = tail_vertex->edge_list;

  /* First add tail_vertex and all previous nodes to the head_vertex */
  assert(head_vertex && tail_vertex);
  insert_at_head(&head_vertex->merged_vertices, tail_vertex->key);
  // TODO : Make append take a null head.
  append(head_vertex->merged_vertices, tail_vertex->merged_vertices);

  /* Iterate through each neighbor of the tail */
  while(tail_edges) {
    // Get edge list of the neighbor.
    node_t* neighbor_edge_list_it = _get_neighbors_edges(tail_edges);

    // If reference to tail, make it a reference to the head
    while(neighbor_edge_list_it) {
      if(tail_vertex == _get_neighbor_vertex(neighbor_edge_list_it)) {
        _change_tail_of_edge_node(neighbor_edge_list_it, head_vertex);
      }
      neighbor_edge_list_it = neighbor_edge_list_it->next;
    }
    /* Change head of this edge to head vertex */
    _change_head_of_edge_node(tail_edges, head_vertex);
    tail_edges = tail_edges->next;
  }

  /* By here, we are ready to merge the tail vertex list into the head list */
  assert(head_vertex);

  append(head_vertex->edge_list, tail_vertex->edge_list);

  /* Need to remove old vertex, and old edge! */
  memset(tail_vertex, 0, sizeof(vertex_t));
  graph->number_of_vertices--;

  return head_vertex;
 }

/* Used as call back to compare an edge node key and the tail vertex are the same */
int 
_compare_vertice_nodes_cb(void *node_key, void *search_key) {
  
  if(!node_key || !search_key) {
    return node_key == search_key;
  }

  edge_t *edge = node_key;
  vertex_t *tail_vertex = edge->tail_vertex;
  if (tail_vertex == search_key) {
    return 1;
  }
  // This shouldn't happen, but it's a possiblility if a bug occurs.
  // This would mean the edge is a loop, but is in the wrong vertex list.
  assert(!(edge->head_vertex == edge->tail_vertex));
  
  // Generally they'll be different, though.
  return 0;
}

/* Used to find a specific edge node in an edge list */
int
_compare_edge_nodes_cb(void *node_key, void *search_key) {
    return node_key == search_key;
}

/* Delete a node when a loop is detected during the
 *  Basically:
 *  1. Copy last of edge node to edge (effectively deleting the node)
 *  2. Go to head and find the key pointing to the end node, replace
 *     key with new edge (tail will be fine)
 *  3. Null out last edge and decrement count by 1.
 *  4. Finally remove the edge node in the graph (done after call back).
 */
void
_delete_edge(graph_t *graph, node_t *edge_node) {
  if(!graph || !edge_node)
      return;

  edge_t *edge = edge_node->key;
  if(!edge || !graph->edges)
    return;
 
  edge_t *last_edge_node = &graph->edges[graph->number_of_edges-1];
  
  // If we're erasing last edge in node then just null and return.
  if (last_edge_node == edge) {
    _change_head_of_edge(edge, NULL);
    _change_tail_of_edge(edge, NULL);
    graph->number_of_edges--;
    return;
  }
   

  // We need to verify that edge is in graph.
  int is_valid_edge = _verify_address_in_range(
                          (unsigned long long) graph->edges,
                          (unsigned long long) sizeof(edge_t) * 
                                               graph->number_of_edges,
                          (unsigned long long) edge);

  assert(is_valid_edge);

  // Assign last_edge_node head/tail to the one that should be removed
  _change_head_of_edge(edge, last_edge_node->head_vertex);
  _change_tail_of_edge(edge, last_edge_node->tail_vertex);
  // Zero out last element.
  _change_head_of_edge(last_edge_node, NULL);
  _change_tail_of_edge(last_edge_node, NULL);
  graph->number_of_edges--;
  // Update the head of swapped edge to point to new index.
  vertex_t *swapped_vertex = edge->head_vertex;
  assert(swapped_vertex);
  node_t *stale_edge_node = find_key(swapped_vertex->edge_list,
                                     last_edge_node,
                                     _compare_edge_nodes_cb);

  // Should have found it.
  assert(stale_edge_node);
  
  // Now update so the key of stale edge node points to updated edge.
  stale_edge_node->key = edge;
}


/* Removes self-referring edges in a edge list.
 * Does so in O(k) time, where k is number of edges of the node, so
 * essentially O(1) time for N edges overall.
 */
void
remove_loops_of_vertex(graph_t *graph, vertex_t *v) {
  if(!graph || !v) {
    return;
  }
  node_t *loop_edge_node = NULL;

  // Search through list and compare edge_node to it's tail vertex
  while((loop_edge_node = 
                find_key(v->edge_list, v, _compare_vertice_nodes_cb)))
  {
    // Remove edge from graph.
    _delete_edge(graph, loop_edge_node);
    // Remove edge node from the list.
    delete_node(&(v->edge_list), loop_edge_node);
  }
}

vertex_t*
merge_edge_and_remove_loops(graph_t *graph, int edge_n) {
  if (!graph || edge_n > graph->number_of_edges)
    return NULL;
  else {
    vertex_t *v = merge_vertices_of_edge(graph, &graph->edges[edge_n]);
    remove_loops_of_vertex(graph, v);
    return v;
  }
}

