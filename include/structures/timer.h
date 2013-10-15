#pragma once

#include "time.h"

typedef struct {
  clock_t last, total;
} timer_t;

void timer_init(timer_t * timer);
void timer_start(timer_t * timer);
void timer_end(timer_t * timer);
double timer_total(const timer_t * timer);