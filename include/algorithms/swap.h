#pragma once

#define SWAP(a, b) do { __typeof(a) t = a; a = b; b = t; } while(0)