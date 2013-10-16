#include "algorithms/binary_search.h"

uint32_t * binary_search(uint32_t * begin, uint32_t * end,
                         uint32_t value) {
  ptrdiff_t l = -1, r = (end - begin);
  while(r - l > 1) {
    const ptrdiff_t m = (l + r) >> 1;
    if(*(begin + m) < value) {
      l = m;
    } else {
      r = m;
    }
  }
  return (begin + r);
}