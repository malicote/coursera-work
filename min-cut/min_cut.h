#ifndef _MIN_CUT_H
#define _MIN_CUT_H

#include <time.h>
#include <stdlib.h>
#include "graph.h"
#include "limits.h"
/* Function to find a min cut of graph */
long long find_a_min_cut(graph_t *graph);

/* Function to find min cut of "tries" iterations. */
long long find_min_cut(graph_t *graph, long long tries);
#endif
