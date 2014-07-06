#include "min_cut.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Find a min cut.  Algorithm:
 * 1. Select a random edge.
 * 2. Collapse the two vertices connected by the edge.
 * 3. Repeat until number of vertices is 2.
 * Returns number of edges (i.e. min-cut)
 */
long long find_a_min_cut(graph_t *graph) {
  if(!graph)
    return -1;

  while(graph->number_of_vertices > 2) {
    long long random_edge = rand() % MAX(graph->number_of_edges, 0);
    merge_edge_and_remove_loops(graph, random_edge); 
    
  }
  return graph->number_of_edges;
}

long long find_min_cut(graph_t *graph, long long tries) {
  long long i = 0;
  long long min_cut = LLONG_MAX;
  for(i = 0; i < tries; i++) {
    int mc = find_a_min_cut(graph);
//    printf("mc=%ing\n", mc);
    assert(mc > 0);
    min_cut = MIN(mc, min_cut);
  }
  return min_cut;
}



