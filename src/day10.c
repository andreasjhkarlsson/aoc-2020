#include "adventofcode.h"
#include "util.h"

#define MAX_INPUT_LENGTH 128

int find_perfect_chain(int input, int target, int jolts[], int jolts_length, int one_diffs, int three_diffs)
{
    int diff = target - input;
    if (diff <= 3)
    {
        if (diff==1) one_diffs++;
        if (diff==3) three_diffs++;
        return one_diffs * three_diffs;
    }
    for(int i=0; i<jolts_length; i++)
    {
        if (jolts[i] < input)
            continue;
        int diff = jolts[i] - input;
        if (diff <= 3)
        {
            int result = find_perfect_chain(
                jolts[i],
                target,
                &jolts[i+1],
                jolts_length-i-1,
                one_diffs + (diff==1?1:0),
                three_diffs + (diff==3?1:0)
            );
            if (result!=-1)
                return result;
        }
    }

    return -1;
}

int64_t count_chains(int input, int target, int jolts[], int jolts_length, int64_t cache[])
{
    if (cache[input]!=-1)
        return cache[input];

    if (target - input <= 3)
        return 1;

    int64_t res = 0;

    for (int i=0;i<jolts_length;i++)
    {
        int diff = jolts[i] - input;
        if (diff > 3)
            break;
        
        res += count_chains(jolts[i], target, &jolts[i+1], jolts_length-i-1, cache);
    }

    cache[input] = res;

    return res;
}

int main(void)
{
    DAY(10, "Adapter Array")

    int input[MAX_INPUT_LENGTH];
    int input_length = 0;

    while (peekchar()!=EOF)
        scanf("%i\n", &input[input_length++]);
    
    sqsort(input, input_length);

    int target_joltage = input[input_length-1] + 3;

    int64_t part1 = find_perfect_chain(0, target_joltage, input, input_length, 0, 0);

    int64_t cache[target_joltage+1];
    memset64(cache, -1LL, target_joltage+1);

    int64_t part2 = count_chains(0, target_joltage, input, input_length, cache);

    SOLUTION(part1, part2)
}