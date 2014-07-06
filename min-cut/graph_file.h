#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "list.h"

int add_int_to_list(node_t **head, int key) {
  if (!head)
    return -1;

  int *keyp = malloc(sizeof key);
  assert(key);
  
  *keyp = key;
  
  insert_at_tail(head, keyp);

  return 0;
}

void print_table(void **v_list, long long v_count, void ***n_list, int *n_count);

/* This file is used to parse "graph files", text files
   that hold graph information.

   Expected structue for M vertices, each with N-M edges:
   v1 e1-1 e1-2 e1-3 ...
   v2 e2-1 e2-2 e2-3 ...
   ...
   vM eM-1 eM-2 eM-3 ...
*/
int 
_parse_graph_file(const char *path, graph_t *graph, int n) {
  assert(graph && path);

  FILE *graph_file = fopen(path, "r");
  if (!graph_file) {
    printf("ERROR opening %s : %s\n", path, strerror(errno));
    return -1;
  }
 
  long long vlist_size = 256;
  void *vertex_list[vlist_size];
  memset(vertex_list, 0, vlist_size * sizeof(void*));

  void **neighbor_list[vlist_size];
  memset(neighbor_list, 0, vlist_size * sizeof(void*));

  int number_of_neighbors[vlist_size];
  memset(number_of_neighbors, 0, vlist_size * sizeof(int));

  int vertex_count = 0;
  char *line = NULL;
  size_t line_length = 0;

  printf("Starting to parse graph file...\n"); 
  // Read a line
  while ((getline(&line, &line_length, graph_file)) != -1) {
    // Parse into correct vertex and its neighbors.
    // For shortcut, assume no vertex will have more than 128 neighbors. 
    char *head = line;
    char *tail = NULL;
    int key = strtol(line, &tail, 10);

    // Get vertex.
    if (tail && tail!=head) {
      vertex_list[vertex_count] = malloc(sizeof(int));
      assert(vertex_list[vertex_count]);
      *(int *)vertex_list[vertex_count] = key;
    } else {
      continue;
    }
    head = tail;
    node_t *temp_edge_list = NULL;
    // Get neighbors.
    int neighbor = strtol(head, &tail, 10);
    while (tail && tail!=head) {
      add_int_to_list(&temp_edge_list, neighbor);
      head = tail;
      neighbor = strtol(head, &tail, 10);
    }
    // Have neighbors in list; now need to get the array and store in neighbor list.
    int list_size = 0;
    neighbor_list[vertex_count] = arrayitize(temp_edge_list,
                                             &list_size,       
                                             &number_of_neighbors[vertex_count]);
    vertex_count++;
    assert(vertex_count < vlist_size); 
  }
  fflush(stdout);
  printf("Done parsing, exited with %s\n", strerror(errno));
  free(line);
  
 
  print_table(vertex_list, vertex_count, neighbor_list, number_of_neighbors);
  int g = 0;
  for(g = 0; g < n; g++) {
    add_vertices(&graph[g], vertex_list, vertex_count, neighbor_list, number_of_neighbors);
  }
  return 0;
}

// Initialize single graph with file.
int 
parse_graph_file(const char *path, graph_t *graph) {
  return _parse_graph_file(path, graph, 1);
}

// Initialize n graphs to the same file without parsing the file every iteration.
int 
parse_graph_file_vector(const char *path, graph_t *graph, int n) {
  return _parse_graph_file(path, graph, n);
}

// Assumes keys are ints.
void print_table(void **v_list, long long v_count, void ***n_list, int *n_count) {
  int v = 0;
  int e = 0;
  printf("--------------COMPLETED TABLE-------------\n");
  printf("Number of vertices: %lld\n", v_count);
  for (v = 0; v < v_count; v++) {
    printf("[%d] :", *(int *)v_list[v]);
    for (e = 0; e < n_count[v]; e++) {
      printf(" [%d] ", *(int *)n_list[v][e]);
    }
    printf("\n");
  }
}
