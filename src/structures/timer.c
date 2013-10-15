#include "structures/timer.h"

void timer_init(timer_t * timer) {
  timer->last = 0;
  timer->total = 0;
}

void timer_start(timer_t * timer) {
  timer->last = clock();
}

void timer_end(timer_t * timer) {
  timer->total += (clock() - timer->last);
  timer->last = 0;
}

double timer_total(const timer_t * timer) {
  return (double) (timer->total) / CLOCKS_PER_SEC;
}