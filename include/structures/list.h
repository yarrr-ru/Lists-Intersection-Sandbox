#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
  uint32_t size;
  uint32_t * data;
} list_t;

int list_read_file(FILE * file, list_t * list);
uint32_t list_sz(list_t * list);
uint32_t list_at(list_t * list, uint32_t index);