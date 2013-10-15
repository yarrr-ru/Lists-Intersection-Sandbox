#pragma once

#include "structures/list.h"

typedef uint32_t (*intersector_f)(uint32_t lists_sz, list_t * lists,
                                  uint32_t how_much, uint32_t * results);