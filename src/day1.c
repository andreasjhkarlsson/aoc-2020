#include "adventofcode.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int find_pair(int list[], size_t length, int last, int target)
{
    if( length==0)
        return -1;

    for(int i=0;i<length;i++)
    {
        if (list[i]+last==target)
            return list[i]*last;
    }

    return find_pair(list+1, length-1, list[0], target);    
}

int find_triple(int list[], size_t length, int target)
{
    for(int i=0;i<length;i++)
    {
        int res = find_pair(list, length, list[i], target-list[i]);

        if (res != -1)
            return res*list[i];
    }
}

int main(void)
{
    DAY(1, "Report Repair")

    int input[4096];
    int input_length = 0;

    while (peekchar()!=EOF)
    {
        input[input_length++] = getint();
        getchar(); // newline
    }

    SOLUTION
    (
        (int64_t)find_pair(input, input_length, 0, 2020),
        (int64_t)find_triple(input, input_length, 2020)
    )
}