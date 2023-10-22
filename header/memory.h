#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <string.h>

#include "types.h"

#define allocate(type) malloc(sizeof(type))
#define allocate_array(type, count) malloc(sizeof(type) * count)
#define free(block) free(block)
#define zero_memory(block, size) memset(block, 0, size);
#define set_memory(block, size, value) memset(block, value, size);

#endif