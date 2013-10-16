#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "intersectors/dumb_bs.h"
#include "algorithms/binary_search.h"


size_t dumb_bs_intersect(size_t lists_sz, const list_t * lists,
                         size_t how_much, uint32_t * result)
{
  // Iterator analog
  typedef uint32_t * iter_t;

  // Only one list, trivial
  if(lists_sz == 1) {
    if(list_sz(&lists[0]) < how_much) {
      how_much = list_sz(&lists[0]);
    }
    memcpy(result, list_begin(&lists[0]), how_much * sizeof(uint32_t));
    return how_much;
  }

  // Init helpers
  iter_t begins[lists_sz], ends[lists_sz];

  for(size_t i = 0; i < lists_sz; i++) {
    begins[i] = list_begin(&lists[i]);
    ends[i] = list_end(&lists[i]);
  }
  
  // Intersect
  size_t sz = 0;
  while(sz < how_much) {
    // Empty list flag
    bool empty_list = false;
    // Get max head
    uint32_t max_head = 0;
    for(size_t i = 0; i < lists_sz; i++) {
      assert(begins[i] <= ends[i]);

      // List is empty
      if(begins[i] == ends[i]) {
        empty_list = true;
      }

      if(*begins[i] > max_head) {
        max_head = *begins[i];
      }
    }

    if(empty_list) {
      break;
    }

    bool ok = true;

    // Iterate heads
    for(size_t i = 0; i < lists_sz; i++) {
      if(*begins[i] < max_head) {
        begins[i] = binary_search(begins[i], ends[i], max_head);
      }
      if(begins[i] < ends[i] && *begins[i] == max_head) {
        ++begins[i];
      } else {
        ok = false;
      }
    }

    if(ok) {
      result[sz++] = max_head;
    }
  }


  return sz;
}