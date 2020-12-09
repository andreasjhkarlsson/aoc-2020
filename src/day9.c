#include "adventofcode.h"
#include "util.h"

#define PREAMBLE_SIZE 25

bool find_pair(int64_t value, int64_t pa[])
{
    for(int i=0;i<PREAMBLE_SIZE;i++)
    {
        for(int j=i+1;j<PREAMBLE_SIZE;j++)
        {
            if ((pa[i] + pa[j]) == value)
            {
                return true;
            }
        }
    }
    return false;
}

int64_t has_prefix_sum(int64_t target, int64_t values[], size_t values_length, int64_t smallest, int64_t greatest)
{
    if (values_length==0)
    {
        return -1;
    }

    int64_t value = values[0];

    if (value > greatest)
        greatest = value;
    else if (value < smallest)
        smallest = value;
    
    if (value==target)
        return smallest+greatest;
    else if (value > target)
        return -1;
    else
        return has_prefix_sum(target-value, &values[1], values_length-1, smallest, greatest);
}

int main(void)
{
    DAY(9, "Encoding Error")

    int64_t input [1024];
    int64_t input_length = 0;
    while(peekchar()!=EOF)
    {
        scanf("%"PRId64"\n", &input[input_length++]);
    } 
    
    int64_t preamble[PREAMBLE_SIZE];
    memcpy(preamble, input, PREAMBLE_SIZE * 8);

    int64_t part1 = 0LL;
    int64_t part2 = 0LL;

    for(int i=PREAMBLE_SIZE;i<input_length;i++)
    {
        if (!find_pair(input[i], preamble))
        {
            part1 = input[i];
            break;
        }
        preamble[i%PREAMBLE_SIZE] = input[i];
    }

    for(int i=0;i<input_length-1;i++)
    {
        part2 = has_prefix_sum(part1 - input[i], &input[i+1], input_length-i-1, input[i],input[i]);
        if (part2!=-1)
        {
            break;

        }
        
    }

    SOLUTION(part1, part2);
}