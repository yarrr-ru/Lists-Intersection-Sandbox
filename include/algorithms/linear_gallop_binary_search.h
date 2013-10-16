#pragma once

#include <stdint.h>
#include <stddef.h>

uint32_t * linear_gallop_binary_search(uint32_t * begin, uint32_t * end,
                                       uint32_t value);