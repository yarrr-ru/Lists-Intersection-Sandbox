#include "algorithms/binary_search.h"
#include "algorithms/gallop_binary_search.h"
#include "algorithms/linear_gallop_binary_search.h"

#define LINEAR_STEPS 2

uint32_t * linear_gallop_binary_search(uint32_t * begin, uint32_t * end,
                                       uint32_t value) {
  for(size_t step = 0;
      step < LINEAR_STEPS
      && begin < end
      && *begin < value;
      ++step, ++begin)
    ;

  if(begin < end) {
    return ((*begin >= value) 
            ? begin :
            gallop_binary_search(begin, end, value));
  } else {
    return end;
  }
}