#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "intersectors/mail_ru_improved.h"
#include "algorithms/min_max.h"
#include "algorithms/swap.h"
#include "algorithms/binary_search.h"
#include "algorithms/gallop_binary_search.h"
#include "algorithms/linear_gallop_binary_search.h"

size_t mail_ru_improved_intersect(size_t lists_sz, const list_t * lists,
                                  size_t how_much, uint32_t * result)
{
  // Iterator analog
  typedef uint32_t * iter_t;

  // Only one list, trivial
  if(lists_sz == 1) {
    how_much = MIN(how_much, list_sz(&lists[0]));
    memcpy(result, list_begin(&lists[0]), how_much * sizeof(uint32_t));
    return how_much;
  }

  #ifdef DEBUG
    printf("Start intersection session\n");
    printf("Lists count: %lu\n", lists_sz);
  #endif

  // Init helpers
  iter_t begins[lists_sz], ends[lists_sz];

  #ifdef DEBUG
    printf("Lists sizes:\n");
  #endif

  for(size_t i = 0; i < lists_sz; i++) {
    begins[i] = list_begin(&lists[i]);
    ends[i] = list_end(&lists[i]);
    assert(begins[i] <= ends[i]);
    #ifdef DEBUG
      printf("%lu\n", (ends[i] - begins[i]));
    #endif
    // Empty list
    if(begins[i] == ends[i]) {
      return 0;
    }
  }

  #ifdef DEBUG
    printf("Min-tail-max-head step\n");
  #endif

  // Get max head and min tail
  uint32_t max_head = 0, min_tail = (uint32_t) (-1);
  for(size_t i = 0; i < lists_sz; i++) {
    // Update max head and min tail
    if(*begins[i] > max_head) {
      max_head = *begins[i];
    }
    if(*(ends[i] - 1) < min_tail) {
      min_tail = *(ends[i] - 1);
    }
    #ifdef DEBUG
      printf("%u %u\n", max_head, min_tail);
    #endif
    // Max head larger than min tail,
    // obvious empty intersection
    if(max_head > min_tail) {
      return 0;
    }
  }

  //printf("Update tail-head step\n");

  // Update begins and ends
  for(size_t i = 0; i < lists_sz; i++) {
    begins[i] = gallop_binary_search(begins[i], ends[i], max_head);
    ends[i] = binary_search(begins[i], ends[i], min_tail + 1);
    assert(begins[i] <= ends[i]);
    #ifdef DEBUG
      printf("%lu\n", (ends[i] - begins[i]));
    #endif
    // Empty list after updates
    if(begins[i] == ends[i]) {
      return 0;
    }
  }
  
  // Cursor to current list in round-robbin
  size_t idx = 0;
  // Intersection size
  size_t sz = 0;
  // Interesting elements
  uint32_t current_id = max_head, max_next_id = max_head;
  // How many lists contains current_id element
  size_t matches = 0;

  // Pick up start list (with max head)
  for(size_t i = 0; i < lists_sz; i++) {
    if(*begins[i] >= current_id) {
      idx = (i + 1) % lists_sz;
      current_id = *(begins[i]);
      max_next_id = ((begins[i] + 1 < ends[i])
                     ? MAX(max_next_id, *(begins[i] + 1))
                     : ((uint32_t) -1));
      matches = 1;
    }
  }

  // Main intersection phase
  #ifdef DEBUG
    printf("Main step\n");
    size_t total_iterations = 0;
  #endif

  while(sz < how_much) {
    #ifdef DEBUG
      ++total_iterations;
      printf("Values: %lu %u %lu %u\n", idx, current_id, matches, max_next_id);
      const size_t size_before = ends[idx] - begins[idx];
      printf("Size before: %lu\n", size_before);
    #endif

    // Search new head
    begins[idx] = linear_gallop_binary_search(begins[idx], ends[idx], current_id);
    assert(begins[idx] <= ends[idx]);

    #ifdef DEBUG
      const size_t size_after = ends[idx] - begins[idx];
      printf("Size after and jump: %lu %lu\n", size_after, (size_before - size_after));
    #endif

    // Got empty list
    if(begins[idx] == ends[idx]) {
      break;
    }

    // Found element
    if(*begins[idx] == current_id) {
      ++matches;

      #ifdef DEBUG
        printf("++matches\n");
      #endif

      if(matches == lists_sz) {
        result[sz++] = current_id;

        #ifdef DEBUG
          printf("Found new result value: %u\n", current_id);
        #endif

        // Move list head to next value
        ++begins[idx];
        // Got empty list
        if(begins[idx] == ends[idx]) {
          break;
        }
        // Get new current_id
        current_id = MAX(*begins[idx], max_next_id);
        matches = ((*begins[idx] == current_id) ? 1 : 0);
      }
    } else {
      // Get new current_id
      current_id = MAX(*begins[idx], max_next_id);
      matches = ((*begins[idx] == current_id) ? 1 : 0);
    }

    // Try to update max_next_id
    max_next_id = ((begins[idx] + 1 < ends[idx])
                   ? MAX(*(begins[idx] + 1), max_next_id)
                   : ((uint32_t) -1));

    // Go to next list (round-robbin)
    idx = (idx + 1) % lists_sz;
  }

  #ifdef DEBUG
    printf("End: %lu %lu\n", sz, total_iterations);
  #endif
  
  return sz;
}