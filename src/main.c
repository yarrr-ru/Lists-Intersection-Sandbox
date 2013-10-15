#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "interfaces/intersector.h"
#include "structures/lists.h"
#include "algorithms/dumb.h"

// File with lists
static const char * lists_fn = "../data/lists.dat";

// File with queries to lists
static const char * queries_fn = "../data/list_queries.dat";

// How much we take from intersection
static const uint32_t intersection_sz = 80;

int main(void) {
  // Read lists
  lists_t lists, queries;
  lists_read(lists_fn, &lists);
  lists_read(queries_fn, &queries);

  // Algorithms
  intersector_f algorithms[1] = {dumb_intersect};
  uint32_t algorithms_sz = sizeof(algorithms) / sizeof(algorithms[0]);

  // Memory for results
  uint32_t results[algorithms_sz][intersection_sz], result_sizes[algorithms_sz];

  // Intersect
  for(uint32_t query_id = 0; query_id < lists_sz(&queries); query_id++) {
    // Get lists for intersection
    list_t query = lists_at(&queries, 1);
    list_t lists_to_intersect[list_sz(&query)];
    for(uint32_t i = 0; i < list_sz(&query); i++) {
      lists_to_intersect[i] = lists_at(&lists, list_at(&query, i));
    }
    // Clear memory
    for(uint32_t i = 0; i < algorithms_sz; i++) {
      memset(results[i], 0, sizeof(uint32_t) * intersection_sz);
    }
    // Intersect
    for(uint32_t i = 0; i < algorithms_sz; i++) {
      intersector_f algorithm = algorithms[i];
      result_sizes[i] = 
        algorithm(list_sz(&query), lists_to_intersect, intersection_sz, results[i]);
      assert(result_sizes[i] <= intersection_sz);
    }
    // Check results
    for(uint32_t i = 1; i < algorithms_sz; i++) {
      assert(result_sizes[i - 1] == result_sizes[i]);
      assert(memcmp(results[i - 1], results[i],
                    sizeof(uint32_t) * result_sizes[i]) == 0);
    }
  }

  // Print timer results

  return 0;
}