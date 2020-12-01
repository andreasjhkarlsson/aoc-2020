#include "days.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int find_pair(const char *lines, int last, int target)
{
    const char* line = lines;

    if( *line=='\0')
        return -1;
    
    while (*line)
    {
        size_t line_length = strcspn(line, "\n\0");
        int n = atoi_s(line, line_length);
        if (n+last==target)
            return n*last;
        
        line = line + line_length + 1;
    }

    line = lines;
    size_t line_length = strcspn(line, "\n\0");
    int n = atoi_s(line, line_length);

    return find_pair(line+line_length+1, n, target);    
}

int find_triple(const char *lines, int target)
{
    const char* line = lines;
    
    while (*line)
    {
        size_t line_length = strcspn(line, "\n\0");
        int n = atoi_s(line, line_length);
        
        int res = find_pair(lines, n, target-n);

        if (res != -1)
            return res*n;        
        
        line = line + line_length + 1;
    }

}

struct solution day1(const char* lines)
{
    struct solution solution = {0, 0};

    solution.part1 = find_pair(lines, 0, 2020);

    solution.part2 = find_triple(lines, 2020);

    return solution;
}