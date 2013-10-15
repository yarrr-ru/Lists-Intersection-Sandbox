#include <assert.h>
#include <stdbool.h>

#include "algorithms/dumb.h"

size_t dumb_intersect(size_t lists_sz, const list_t * lists,
                      size_t how_much, uint32_t * result)
{
  // Iterator analog
  typedef uint32_t * iter_t;

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
      while(begins[i] < ends[i] && *begins[i] < max_head) {
        ++begins[i];
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