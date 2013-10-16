#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include "interfaces/intersector.h"
#include "algorithms/min_max.h"
#include "structures/lists.h"
#include "structures/timer.h"
#include "intersectors/dumb.h"
#include "intersectors/dumb_bs.h"
#include "intersectors/mail_ru.h"
#include "intersectors/mail_ru_improved.h"
#include "intersectors/baeza_yates.h"

#define LISTS_FN          "../data/lists.dat"
#define QUERIES_FN        "../data/queries.dat"
#define INTERSECTION_SZ   128
#define QUERIES_LIMIT     100000
#define ITS_PER_LIST      1

int main(void) {
  // Read lists
  lists_t lists, queries;
  lists_read(LISTS_FN, &lists, true);
  lists_read(QUERIES_FN, &queries, false);

  // Algorithms
  const intersector_f algorithms[3] =
    {
     /*dumb_intersect,
     dumb_bs_intersect,*/
     mail_ru_intersect,
     mail_ru_improved_intersect,
     baeza_yates_intersect
    };
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
  const size_t max_query_id = MIN(lists_sz(&queries), QUERIES_LIMIT);
  size_t iterations = 0;

  // Intersect
  for(size_t query_id = 0; query_id < max_query_id; query_id++) {
    // Get lists for intersection
    const list_t query = lists_at(&queries, query_id);
    const size_t lists_in_query = list_sz(&query);
    if(lists_in_query != 5) continue;
    list_t lists_to_intersect[lists_in_query];
    for(size_t i = 0; i < lists_in_query; i++) {
      lists_to_intersect[i] = lists_at(&lists, list_at(&query, i));
    }
    for(size_t it = 0; it < ITS_PER_LIST; it++) {
      // Intersect
      ++iterations;
      for(size_t i = 0; i < algorithms_sz; i++) {
        // Round-robbin per algorithms
        // For better results (cause CPU-cache etc)
        const size_t algorithm_idx = (query_id + it + i) % algorithms_sz;
        intersector_f algorithm = algorithms[algorithm_idx];
        timer_start(&timers[algorithm_idx]);
        result_sizes[algorithm_idx] = 
          algorithm(lists_in_query, lists_to_intersect,
                    INTERSECTION_SZ, results[algorithm_idx]);
        timer_end(&timers[algorithm_idx]);
        assert(result_sizes[algorithm_idx] <= INTERSECTION_SZ);
      }
      // Check results
      for(size_t i = 1; i < algorithms_sz; i++) {
        assert(result_sizes[i - 1] == result_sizes[i]);
        assert(memcmp(results[i - 1], results[i],
                      sizeof(uint32_t) * result_sizes[i]) == 0);
      }
    }
  }

  // Print timer results
  for(size_t i = 0; i < algorithms_sz; i++) {
    const double lists_per_sec =
      (double) (iterations) / timer_total(&timers[i]);
    printf("%lf\n", lists_per_sec);
  }

  return 0;
}