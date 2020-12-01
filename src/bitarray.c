#include "bitarray.h"


void bit_array_init(void* vector, size_t length)
{
    memset(vector, 0, length);
}

void bit_array_set(void* vector, int bit)
{
    uint32_t* word = ((uint32_t*)vector) + bit / 32;
    *word |= 1 << (bit % 32);    
}

void bit_array_clear(void* vector, int bit)
{
    uint32_t* word = ((uint32_t*)vector) + bit / 32;
    *word &= ~(1 << (bit % 32));    
}

bool bit_array_is_set(void* vector, int bit)
{
    uint32_t* word = ((uint32_t*)vector) + bit / 32;
    return (*word >> bit) & 1;
}
