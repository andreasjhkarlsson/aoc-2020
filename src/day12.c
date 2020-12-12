#include "adventofcode.h"
#include <stdlib.h>


int isin(int d)
{
    while (d<0)
        d+=360;
    switch (d % 360)
    {
    case 0: return 0;
    case 90: return 1;
    case 180: return 0;
    case 270: return -1;
    default: BAIL("Invalid arg")
    }
}

int icos(int d)
{
    return isin(d+90);
}

struct instruction
{
    enum type
    {
        N = 'N',
        S = 'S',
        E = 'E',
        W = 'W',
        L = 'L',
        R = 'R',
        F = 'F'
    } type;

    int arg;
};

struct point
{
    int x, y;
};

struct ship
{
    struct point pos;

    int direction;
};

struct waypoint
{
    struct point pos;
};

struct point rotate(struct point p, int a)
{
    char matrix [2][2] =
    {
        { icos(a), -isin(a) },
        { isin(a), icos(a)  }
    };

    struct point result = {
        p.x * matrix[0][0] + p.y * matrix[0][1],
        p.x * matrix[1][0] + p.y * matrix[1][1]
    };
    
    return result;
}

void navigate(struct ship* ship, struct instruction instruction)
{
    switch (instruction.type)
    {
    case N:
        ship->pos.y -= instruction.arg;
        break;
    case S:
        ship->pos.y += instruction.arg;
        break;
    case W:
        ship->pos.x -= instruction.arg;
        break;
    case E:
        ship->pos.x += instruction.arg;
        break;
    case F:
        ship->pos.x += icos(ship->direction) * instruction.arg;
        ship->pos.y += isin(ship->direction) * instruction.arg;
        break;
    case L:
        ship->direction -= instruction.arg;
        break;
    case R:
        ship->direction += instruction.arg;
        break;
    default:
        BAIL("Unexpected type")
    }
}

void navigate_waypoint(struct ship *ship, struct waypoint* waypoint, struct instruction instruction)
{
    switch (instruction.type)
    {
    case F:
        ship->pos.x += waypoint->pos.x * instruction.arg;
        ship->pos.y += waypoint->pos.y * instruction.arg;
        break;
    case N:
        waypoint->pos.y -= instruction.arg;
        break;
    case S:
        waypoint->pos.y += instruction.arg;
        break;
    case E:
        waypoint->pos.x += instruction.arg;
        break;
    case W:
        waypoint->pos.x -= instruction.arg;
        break;
    case R:
        waypoint->pos = rotate(waypoint->pos, instruction.arg);  
        break;
    case L:
        waypoint->pos = rotate(waypoint->pos, -instruction.arg);     
        break;
    
    }
}

int main(void)
{
    DAY(12, "Rain Risk")

    static struct instruction instructions[1024];
    static size_t instructions_length = 0;

    while (peekchar()!=EOF)
    {
        scanf(
            "%c%d\n",
            &instructions[instructions_length].type,
            &instructions[instructions_length].arg
        );
        instructions_length++;
    }

    int64_t part1 = 0, part2 = 0;

    {
        struct ship ship = { {0,0}, 0 };

        for(int i=0;i<instructions_length;i++)
            navigate(&ship, instructions[i]);

        part1 = abs(ship.pos.x) + abs(ship.pos.y);
    }

    {
        struct ship ship = { {0,0}, 0 };
        struct waypoint waypoint = { {10, -1} };

        for(int i=0;i<instructions_length;i++)
            navigate_waypoint(&ship, &waypoint, instructions[i]);

        part2 = abs(ship.pos.x) + abs(ship.pos.y);
    }

    SOLUTION(part1, part2)
}