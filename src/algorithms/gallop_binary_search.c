#include "algorithms/binary_search.h"
#include "algorithms/gallop_binary_search.h"

uint32_t * gallop_binary_search(uint32_t * begin, uint32_t * end,
                                uint32_t value) {
  ptrdiff_t jump = 1;

  while(begin + jump < end && *(begin + jump) <= value) {
    begin += jump;
    jump <<= 1;
  }

  if(begin + jump < end) {
    end = begin + jump;
  }

  return binary_search(begin, end, value);
}