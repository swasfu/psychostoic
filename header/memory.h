#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <string.h>

#include "types.h"

#define ALLOC(type) malloc(sizeof(type))
#define ALLOC_ARRAY(type, count) malloc(sizeof(type) * count)
#define ZERO_MEMORY(block, size) memset(block, 0, size);

#endif