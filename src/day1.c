#include "adventofcode.h"
#include "util.h"
#include "bitarray.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int find_pair(void *set, int target)
{
    for(int n=0;n<target;n++)
    {
        if (bit_array_is_set(set, n) && bit_array_is_set(set, target - n))
            return n * (target - n);
    }

    return -1; 
}

int find_triple(void *set, int target)
{
    for(int n=0;n<target;n++)
    {
        if (!bit_array_is_set(set, n))
            continue;

        int res = find_pair(set, target-n);

        if (res != -1)
            return res*n;
    }

    abort();
}

#define TARGET 2020

int main(void)
{
    DAY(1, "Report Repair")

    // Store numbers in bit vector
    uint32_t numbers[TARGET/32 + 1] = { 0 };

    while (peekchar()!=EOF)
    {
        bit_array_set(numbers, getint());
        getchar(); // newline
    }

    SOLUTION
    (
        (int64_t)find_pair(numbers, TARGET),
        (int64_t)find_triple(numbers, TARGET)
    )
}