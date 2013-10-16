#include <string.h>
#include <assert.h>

#include "intersectors/baeza_yates.h"
#include "algorithms/min_max.h"
#include "algorithms/binary_search.h"

// Iterator analog
typedef uint32_t * iter_t;

static void baeza_yates_intersect_rec(size_t lists_sz,
                                      iter_t * begins, iter_t * ends,
                                      size_t how_much, uint32_t * result,
                                      size_t * sz)
{
  assert(*sz < how_much);

  // Get smallest list
  size_t smallest_idx = 0, smallest_sz = (size_t) -1;
  for(size_t i = 0; i < lists_sz; i++) {
    assert(begins[i] <= ends[i]);
    size_t cur_sz = (ends[i] - begins[i]);
    if(cur_sz == 0) {
      return;
    }
    if(cur_sz < smallest_sz) {
      smallest_idx = i;
      smallest_sz = cur_sz;
    }
  }

  // Only one element remain, check them
  if(smallest_sz == 1) {
    bool found = true;
    const uint32_t value = *(begins[smallest_idx]);
    for(size_t i = 0; i < lists_sz && found; i++) {
      if(i != smallest_idx) {
        uint32_t * ptr = binary_search(begins[i], ends[i], value);
        if(ptr == ends[i] || *ptr != value) {
          found = false;
        }
      }
    }
    if(found) {
      result[(*sz)++] = value;
    }
    return;
  }

  // Get median
  const uint32_t median = *(begins[smallest_idx] + (smallest_sz / 2));

  iter_t splits[lists_sz];
  for(size_t i = 0; i < lists_sz; i++) {
    splits[i] = binary_search(begins[i], ends[i], median);
  }

  // Go to left
  baeza_yates_intersect_rec(lists_sz, begins, splits, how_much, result, sz);

  if(*sz < how_much) {
    // Try to add median to result and fix iterators
    bool median_found = true, need_to_go_right = true;
    for(size_t i = 0; i < lists_sz; i++) {
      assert(splits[i] <= ends[i]);
      if(splits[i] == ends[i] || *splits[i] != median) {
        median_found = false;
      } else {
        ++splits[i];
      }
      if(splits[i] == ends[i]) {
        need_to_go_right = false;
      }
    }

    if(median_found) {
      result[(*sz)++] = median;
    }

    if(need_to_go_right && *sz < how_much) {
      baeza_yates_intersect_rec(lists_sz, splits, ends, how_much, result, sz);
    }
  }
}

size_t baeza_yates_intersect(size_t lists_sz, const list_t * lists,
                             size_t how_much, uint32_t * result)
{
  // Only one list, trivial
  if(lists_sz == 1) {
    how_much = MIN(how_much, list_sz(&lists[0]));
    memcpy(result, list_begin(&lists[0]), how_much * sizeof(uint32_t));
    return how_much;
  }

  // Init helpers
  iter_t begins[lists_sz], ends[lists_sz];

  for(size_t i = 0; i < lists_sz; i++) {
    begins[i] = list_begin(&lists[i]);
    ends[i] = list_end(&lists[i]);
    assert(begins[i] <= ends[i]);
    // Empty list
    if(begins[i] == ends[i]) {
      return 0;
    }
  }

  size_t sz = 0;
  baeza_yates_intersect_rec(lists_sz, begins, ends, how_much, result, &sz);
  return sz;
}