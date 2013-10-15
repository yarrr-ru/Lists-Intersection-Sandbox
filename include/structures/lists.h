#pragma once

#include <stdio.h>
#include <stdint.h>

#include "structures/list.h"

typedef struct {
  uint32_t size;
  list_t * data;
} lists_t;

void lists_read(const char * filename, lists_t * lists, bool check_lists);
size_t lists_sz(const lists_t * list);
list_t lists_at(const lists_t * list, uint32_t index);

