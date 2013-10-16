#pragma once

#include <stdint.h>
#include <stddef.h>

#include "structures/list.h"

size_t baeza_yates_intersect(size_t lists_sz, const list_t * lists,
                             size_t how_much, uint32_t * result);