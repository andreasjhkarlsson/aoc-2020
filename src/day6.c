#include "adventofcode.h"
#include "util.h"

// my boy hamming
// explanation of alg: https://stackoverflow.com/a/109025/242348
int popcnt(uint32_t i)
{
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

int main()
{
    DAY(6, "Custom Customs");

    int64_t total_inclusive_count = 0;
    int64_t total_exclusive_count = 0;

    get_group_answers:
        if (peekchar()==EOF)
            goto done;
        uint32_t inclusive_group_answers = 0;
        uint32_t exclusive_group_answers = 0xFFFFFFFF;

    get_answers:
        uint32_t answers = 0;
        char c;
        next_question:
            if ((c=getchar())!='\n')
            {
                answers |= 1 << (c - 'a');
                goto next_question;
            }

        inclusive_group_answers |= answers;
        exclusive_group_answers &= answers;

        for (c = peekchar(); c != '\n' && c != EOF;)
            goto get_answers;

        total_inclusive_count += popcnt(inclusive_group_answers);
        total_exclusive_count += popcnt(exclusive_group_answers);

        skipchar();

        goto get_group_answers;

    done: SOLUTION(total_inclusive_count,total_exclusive_count);
}