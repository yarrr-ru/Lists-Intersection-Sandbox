#pragma once

#include "structures/list.h"
#include "interfaces/intersector.h"

size_t mail_ru_improved_intersect(size_t lists_sz, const list_t * lists,
                                  size_t how_much, uint32_t * result);