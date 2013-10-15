#pragma once

#include "structures/list.h"

typedef size_t (*intersector_f)(size_t lists_sz, const list_t * lists,
                                size_t how_much, uint32_t * result);