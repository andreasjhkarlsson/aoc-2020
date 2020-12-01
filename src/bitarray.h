#ifndef BITARRAY_H
#define BITARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <inttypes.h>

void bit_array_init(void* vector, size_t length);

void bit_array_set(void* vector, int bit);

void bit_array_clear(void* vector, int bit);

bool bit_array_is_set(void* vector, int bit);

#endif