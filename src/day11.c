#include "adventofcode.h"
#include "util.h"
#include <stdio.h>

enum tile
{
    outside = '\0',
    empty = 'L',
    occupied = '#',
    floor = '.'
};

enum tile get_tile(char grid[], int stride, int x, int y)
{
    return grid[x + y * stride];
}

void set_tile(char grid[], int stride, int x, int y, enum tile tile)
{
    grid[x + y*stride] = tile;
}

struct direction
{
    int x, y;
} directions [] = {
    {-1, -1},
    { 0, -1},
    { 1, -1},
    {-1,  0},
    { 1,  0},
    {-1,  1},
    { 0,  1},
    { 1,  1}
};

enum mode
{
    immediate,
    scan
};

int count_neighbours(char grid[], int stride, int x, int y, enum tile tile, enum mode mode)
{
    int sum = 0;
    for (int i=0;i<sizeof(directions) / sizeof(struct direction);i++)
    {
        struct direction d = directions[i];
        
        int _x = x, _y = y;
        inc:
        _x += d.x;
        _y += d.y;
        enum tile t = get_tile(grid, stride,_x, _y);
        if (t == tile)
        {
            sum++;
            continue;
        }
        if (t==floor && t!=outside && mode==scan)
            goto inc;
    }

    return sum;
}

#define SWAP(A,B) { char* __tmp = A; A = B; B = __tmp; }

size_t stabilize(char grid[], size_t width, size_t height, int tolerance, enum mode mode)
{
    char buffers[2][width*height];

    char* front = &buffers[0][0];
    char* back =  &buffers[1][0];
    
    memcpy(front, &grid[0], width*height);

    bool changed;
    do
    {
        changed = false;
        for(int y=1;y<width-1;y++)
        {
            for(int x=1;x<height-1;x++)
            {
                enum tile tile = get_tile(front, width, x,y);
                enum tile new_tile = tile;
                if (tile == empty && count_neighbours(front, width, x,y, occupied, mode)==0)
                    new_tile = occupied;
                else if (tile == occupied && count_neighbours(front, width, x, y, occupied, mode)>=tolerance)
                    new_tile = empty;

                changed |= tile != new_tile;
                set_tile(back, width, x, y, new_tile);
            }
        }
        
        SWAP(front, back)
        
    } while (changed);


    int sum = 0LL;
    for (int i=0;i<width*height;i++)
    {
        if (front[i] == occupied)
            sum++;
    }

    return sum;
}

#define STRIDE 128
#define MAX_HEIGHT 128

int main(void)
{
    DAY(11, "Seating System")

    char grid[MAX_HEIGHT*STRIDE] = {0};

    for (int row=0;peekchar()!=EOF;row++)
        scanf("%s\n", &grid[STRIDE+1 + row*STRIDE]);

    int64_t part1 = stabilize(grid, STRIDE, MAX_HEIGHT, 4, immediate);

    int64_t part2 = stabilize(grid, STRIDE, MAX_HEIGHT, 5, scan);
    
    SOLUTION(part1, part2);

}
