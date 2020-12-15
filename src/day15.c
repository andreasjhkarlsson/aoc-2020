#include "adventofcode.h"
#include "hashmap.h"

static uint8_t map[48 * 1024 * 1024];

int speak(void* map, int num, int time)
{
    int* last_used_ptr = hashmap_get(map, &num); 
    int last_used =  last_used_ptr ? time - (*last_used_ptr) : 0;

    hashmap_set_int(map, &num, time);
    return last_used;
}

int main(void)
{
    DAY(15, "Rambunctious Recitation")

    hashmap_init(map, sizeof(map), sizeof(int),sizeof(int));

 
    int time = 0;
    int n = 0;
    int starting;
    do
    {
        scanf("%i", &starting);
        n = speak(map, starting, time++);
    } while (getchar()==',');

    int64_t part1, part2;

    for(;;time++)
    {
        if (time==2019)
            part1 = n;
        else if (time==29999999)
        {
            part2 = n;
            break;
        }
        n = speak(map, n, time);
    }
    
    SOLUTION(part1,part2)
}