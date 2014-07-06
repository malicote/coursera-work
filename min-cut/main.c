#include <stdio.h>

#include "graph.h"
#include "list.h"
#include "min_cut.h"
#include "graph_file.h"
#include <time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int compare_ints(void *lhs, void *rhs) {
  if (!lhs || ! rhs) {
    return (lhs == rhs);
  }

  return *(int*)lhs == *(int*)rhs;
}

void print_ints(FILE *buffer, void *key) {
  if(buffer) {
    if(key)
      fprintf(buffer, "%d", *(int*)key);
    else
      fprintf(buffer, "<NULL KEY>");
  }
}

int graph_test();

int main(int argc, char **argv) {
  graph_t my_graph[2000];
  int num_tries = 2000;
  int g = 0;
  for(g = 0; g < num_tries; g++){
    int ret = initialize_graph(&my_graph[g], 
                               201, 
                               10000, 
                               0, 
                               compare_ints, 
                               print_ints);
  }

  if(argc < 2) {
    printf("ERROR: No graph-file path given!\n");
    return 0;
  }

  const char *graph_file_path = argv[1];

  printf("Performing min-cut for graph contained in %s\n", graph_file_path);
  parse_graph_file_vector(graph_file_path, my_graph, num_tries);
  printf("Parsing Done.\n\n\n");

  long long mc = 0;
  long long min_cut = LLONG_MAX;
  srand(time(NULL));
  for(g=0; g < num_tries; g++) {
    if(g %100 == 0) printf("Iteration : %d, min = %lld\n", g, min_cut/2);
    mc = find_a_min_cut(&my_graph[g]);
    min_cut = MIN(min_cut, mc);
  }
  printf("***Min cut of %d tries: %lld ***\n", num_tries, min_cut/2);
  return 0;
}

int graph_test() {
  graph_t my_graph;
  int ret = initialize_graph(&my_graph, 10, 10, 0, compare_ints, print_ints);

  int A = 1;
  int B = 2;
  int C = 3;
  int D = 4;

  void *keys[] = { &A, &B, &C, &D };

  void **neighbors[4] = {
                        /* 1 */ (void* []){&B, &C, &D},
                        /* 2 */ (void* []){&A},
                        /* 3 */ (void* []){&A},
                        /* 4 */ (void* []){&A},
                      };

  int number_of_neighbors[] = {3, 1, 1, 1};

  printf("Init returned: %d\n", ret);

  printf("Creating graph...\n");

  add_vertices(&my_graph, keys, 4, neighbors, number_of_neighbors);

  print_graph(&my_graph, 0xf);

  printf("Finding min cut...\n");
  long long mc = 0;
  long long tries = 100;
  mc = find_min_cut(&my_graph, tries);
  printf("***Min cut of %lld tries: %lld ***\n", tries, mc/2);
  print_graph(&my_graph, 0xf);
   
  return 0;
}
