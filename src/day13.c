#include <string.h>
#include "util.h"
#include "adventofcode.h"

int64_t harmonize(int table[], int table_length)
{
    int64_t t= 0;
    int64_t inc = 1;

    for(int64_t i=0;i<table_length;i++)
    {
        if (table[i]==-1)
            continue;

        for(;;t+=inc)
        {
            for(int j=0;j<(i+1);j++)
            {
                if (table[j]==-1)
                    continue;

                if ((t+j)%table[j]!=0)
                    goto next;
            }
            inc *= table[i];
            break;
            next:;
        }
    }

    return t;
}

int64_t get_closest_departure(int64_t period, int64_t timestamp)
{
    int64_t departure = timestamp + (period - (timestamp % period));
    return departure;
}

int main(void)
{
    DAY(13, "Shuttle Search")
    int table[256];
    memset(table, -1, sizeof(table));

    int earliest = 0;
    scanf("%i\n", &earliest);

    size_t table_length = 0;
    for(int i=0;peekchar()!=EOF;i++)
    {
        if (peekchar()=='x')
            skipchar();
        else
            scanf("%d", &table[table_length]);
        skipchar();
        table_length++;
    }

    int64_t part1 = -1;
    int closest_departure = -1;

    for (int i=0;i<table_length;i++)
    {
        if (table[i]==-1) continue;
        
        int departure = get_closest_departure(table[i], earliest) - earliest;
        if (closest_departure==-1 || departure < closest_departure)
        {
            closest_departure = departure;
            part1 = table[i] * departure;
        }
    }

    int64_t part2  = harmonize(&table[0], table_length);

    SOLUTION(part1, part2)
}