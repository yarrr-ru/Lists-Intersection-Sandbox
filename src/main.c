#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "interfaces/intersector.h"
#include "structures/lists.h"
#include "algorithms/dumb.h"

#define LISTS_FN          "../data/lists.dat"
#define QUERIES_FN        "../data/queries.dat"
#define INTERSECTION_SZ   80

int main(void) {
  // Read lists
  lists_t lists, queries;
  lists_read(LISTS_FN, &lists);
  lists_read(QUERIES_FN, &queries);

  // Algorithms
  const intersector_f algorithms[1] = {dumb_intersect};
  const size_t algorithms_sz = sizeof(algorithms) / sizeof(algorithms[0]);

  // Arrays for results
  uint32_t results[algorithms_sz][INTERSECTION_SZ];
  size_t result_sizes[algorithms_sz];

  // Intersect
  for(size_t query_id = 0; query_id < lists_sz(&queries); query_id++) {
    // Get lists for intersection
    const list_t query = lists_at(&queries, 1);
    const size_t lists_in_query = list_sz(&query);
    list_t lists_to_intersect[lists_in_query];
    for(size_t i = 0; i < lists_in_query; i++) {
      lists_to_intersect[i] = lists_at(&lists, list_at(&query, i));
    }
    // Intersect
    for(size_t i = 0; i < algorithms_sz; i++) {
      intersector_f algorithm = algorithms[i];
      result_sizes[i] = 
        algorithm(list_sz(&query), lists_to_intersect,
                  INTERSECTION_SZ, results[i]);
      assert(result_sizes[i] <= INTERSECTION_SZ);
    }
    // Check results
    for(size_t i = 1; i < algorithms_sz; i++) {
      assert(result_sizes[i - 1] == result_sizes[i]);
      assert(memcmp(results[i - 1], results[i],
                    sizeof(uint32_t) * result_sizes[i]) == 0);
    }
  }

  // Print timer results

  return 0;
}