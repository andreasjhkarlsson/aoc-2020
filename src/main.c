#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "days.h"

struct solution solve(int day, const char* input)
{
    switch (day)
    {
    case 1:
        return day1(input);
    default:
        printf("Day not implemented yet\n");
        abort();
    }
}

int main(int c, char* argv[])
{
    
    if (c!=2)
    {
        printf("Usage: aoc-2020 day\n");
      //  return 1;
    }

    int day = atoi(argv[1]);
    
    if (day>=1 && day<=25)
    {
        static char input_buffer[1024 * 1024]; // 1 MB is hopefully enough
        size_t input_size = 0;
        while (input_size<sizeof(input_buffer)-1)
        {
            int c = getchar();
            if (c==EOF)
                break;
            input_buffer[input_size++] = c;
        }
        input_buffer[input_size++] = '\0';

        struct solution solution = solve(day, input_buffer);
        printf("Part 1: %"PRId64"\nPart 2: %"PRId64"\n", solution.part1, solution.part2);
    }
    else
    {
        printf("Day has to be between 1 and 25\n");
        return 2;
    }
    
    return 0;
}