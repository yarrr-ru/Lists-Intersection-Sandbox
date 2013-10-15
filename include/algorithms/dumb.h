#pragma once

#include "structures/list.h"
#include "interfaces/intersector.h"

uint32_t dumb_intersect(uint32_t lists_sz, list_t * lists,
                        uint32_t how_much, uint32_t * results);