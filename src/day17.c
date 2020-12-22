#include "adventofcode.h"
#include "hashmap.h"
#include "util.h"

enum state
{
    inactive, active
} ACTIVE = active;

enum dimension { _3D, _4D };

struct vec4
{
    int x, y, z, w;
};

struct cell
{
    struct vec4 coord;
    enum state state;
};

struct limits
{
    int xmin, xmax;
    int ymin, ymax;
    int zmin, zmax;
    int wmin, wmax;
};

int active_neighbours_count(struct hashmap* map, struct vec4 coord, enum dimension dim)
{
    int count = 0;

    int wf = dim == _4D ? 1: 0;

    for(int x=-1;x<=1;x++)
    {
        for(int y=-1;y<=1;y++)
        {
            for(int z=-1;z<=1;z++)
            {
                for(int w=-1*wf;w<=1*wf;w++)
                {
                    if (x==0 && y==0 && z==0 && w==0)
                        continue;
                    struct vec4 nb = {coord.x + x, coord.y+y, coord.z+z, coord.w+w};
                    enum state* maybeState = hashmap_get(map, &nb);
                    enum state state = maybeState ? *maybeState: inactive;
                    if (state==active)
                        count++;                  
                }
            }
        }        
    }
    return count;
}

void step(struct hashmap* in, struct hashmap* out, struct limits* limits, enum dimension dim)
{
    limits->xmin--;
    limits->xmax++;
    limits->ymin--;
    limits->ymax++;
    limits->zmin--;
    limits->zmax++;
    if (dim==_4D)
    {
        limits->wmin--;
        limits->wmax++;
    }

    for(int x=limits->xmin;x<=limits->xmax;x++)
    {
        for(int y=limits->ymin;y<=limits->ymax;y++)
        {
            for(int z=limits->zmin;z<=limits->zmax;z++)
            {
                for (int w=limits->wmin;w<=limits->wmax;w++)
                {
                    struct vec4 coord = {x, y, z, w};
                    enum state* maybeState = hashmap_get(in, &coord);
                    enum state state = maybeState ? *maybeState: inactive;
                    int count = active_neighbours_count(in, coord, dim);
                    if (state == active && (count < 2 || count > 3))
                        hashmap_erase(out, &coord);
                    else if(state == inactive && count == 3)
                        hashmap_set(out, &coord, &ACTIVE);
                }
            }
        }
    }
}

int simulate(struct hashmap* initial_state, struct limits limits, int steps, enum dimension dim)
{
    static uint8_t buffers[2][1024 * 1024];

    hashmap_init(&buffers[0][0], sizeof(buffers[0]), sizeof(struct vec4), sizeof(enum state));
    hashmap_init(&buffers[1][0], sizeof(buffers[0]), sizeof(struct vec4), sizeof(enum state));

    hashmap_copy(initial_state, buffers[0]);
    hashmap_copy(initial_state, buffers[1]);

    for (int i=0;i<steps;i++)
    {
        step(buffers[0], buffers[1], &limits, dim);
        hashmap_copy(buffers[1], buffers[0]);
    }

    int64_t count = 0LL;
    struct hashmap_iterator it = hashmap_iterate(buffers[0],NULL);
    
    while (!it.ended)
    {
        if (*((enum state*)it.value) == active)
            count++;
        it = hashmap_iterate(buffers[0], &it);
    }

    return hashmap_count(buffers[0]);
}

int main(void)
{
    DAY(17, "Conway Cubes")

    uint8_t input[2048];

    hashmap_init(input, sizeof(input), sizeof(struct vec4), sizeof(enum state));
    
    struct limits limits = {
        0,0,
        0,0,
        0,0,
        0,0
    };

    for(int y=0;peekchar()!=EOF;y++)
    {
        char c;
        int x = 0;
        while ((c = getchar())!='\n')
        {
            struct cell cell = {
                { x++, y, 0 },
                c == '#' ? active: inactive
            };
            if (cell.state == active)
                hashmap_set(input, &cell.coord, &cell.state);
        }
        limits.xmax = x-1;
        limits.ymax = y;
    }

    int64_t part1 = simulate(input, limits, 6, _3D);
    int64_t part2 = simulate(input, limits, 6, _4D);

    SOLUTION(part1, part2)
}