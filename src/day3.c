#include "adventofcode.h"
#include "util.h"
#include <stdbool.h>

struct slope
{
    char dx, dy;
    short hits;
};

int main(void)
{
    DAY(3, "Toboggan Trajectory")

    struct slope slopes [] =
    {
        {1,1,0},
        {3,1,0},
        {5,1,0},
        {7,1,0},
        {1,2,0}
    };

    for (int row_index=0;peekchar()!=EOF;row_index++)
    {
        uint32_t row = 0; // input is 31 bits wide so fits snuggly
        int cols = 0;

        while (peekchar()!='\n')
        {
            row |= (getchar() == '#' ? 1: 0) << cols++;
        } skipchar();
        
        for(int i=0;i<sizeof(slopes)/sizeof(slopes[0]);i++)
        {
            if (row_index%slopes[i].dy!=0)
                continue; // slope skips this row
            
            // is there a tree at this col?
            if ((row >> (slopes[i].dx * row_index / slopes[i].dy) % cols) & 1)
                slopes[i].hits++;
        }
    }

    int64_t part1 = slopes[1].hits;
    int64_t part2 =
        slopes[0].hits *
        slopes[1].hits *
        slopes[2].hits *
        slopes[3].hits *
        slopes[4].hits;

    SOLUTION(part1, part2)
}