#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include "interfaces/intersector.h"
#include "structures/lists.h"
#include "structures/timer.h"
#include "algorithms/dumb.h"

#define LISTS_FN          "../data/lists.dat"
#define QUERIES_FN        "../data/queries.dat"
#define INTERSECTION_SZ   128
#define QUERIES_LIMIT     10000

int main(void) {
  // Read lists
  lists_t lists, queries;
  lists_read(LISTS_FN, &lists, true);
  lists_read(QUERIES_FN, &queries, false);

  // Algorithms
  const intersector_f algorithms[1] = {dumb_intersect};
  const size_t algorithms_sz = sizeof(algorithms) / sizeof(algorithms[0]);

  // Timers for algorithms
  timer_t timers[algorithms_sz];
  for(size_t i = 0; i < algorithms_sz; i++) {
    timer_init(&timers[i]);
  }

  // Arrays for results
  uint32_t results[algorithms_sz][INTERSECTION_SZ];
  size_t result_sizes[algorithms_sz];

  // Get limit for queries
  const size_t max_query_id = (lists_sz(&queries) < QUERIES_LIMIT)
                               ? lists_sz(&queries) : QUERIES_LIMIT;

  // Intersect
  for(size_t query_id = 0; query_id < max_query_id; query_id++) {
    // Get lists for intersection
    const list_t query = lists_at(&queries, query_id);
    const size_t lists_in_query = list_sz(&query);
    list_t lists_to_intersect[lists_in_query];
    for(size_t i = 0; i < lists_in_query; i++) {
      lists_to_intersect[i] = lists_at(&lists, list_at(&query, i));
    }
    // Intersect
    for(size_t i = 0; i < algorithms_sz; i++) {
      intersector_f algorithm = algorithms[i];
      timer_start(&timers[i]);
      result_sizes[i] = 
        algorithm(lists_in_query, lists_to_intersect,
                  INTERSECTION_SZ, results[i]);
      timer_end(&timers[i]);
      assert(result_sizes[i] <= INTERSECTION_SZ);
    }
    // Check results
    for(size_t i = 1; i < algorithms_sz; i++) {
      assert(result_sizes[i - 1] == result_sizes[i]);
      assert(memcmp(results[i - 1], results[i],
                    sizeof(uint32_t) * result_sizes[i]) == 0);
    }
    // Debug
    /*printf("lists: %lu\tsize: %lu\n", lists_in_query, result_sizes[0]);
    printf("[");
    for(size_t i = 0; i < result_sizes[0]; i++) {
      printf("%u", results[0][i]);
      printf("%c", ",]"[(i + 1) == result_sizes[0]]);
    }
    printf("\n");*/
  }

  // Print timer results
  for(size_t i = 0; i < algorithms_sz; i++) {
    const double lists_per_sec = (double) max_query_id / timer_total(&timers[i]);
    printf("%lf\t%lf\n", timer_total(&timers[i]),
                                   lists_per_sec);
  }

  return 0;
}