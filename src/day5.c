#include "adventofcode.h"
#include "util.h"
#include "bitarray.h"

enum direction
{
    LEFT  = 'L', RIGHT = 'R',
    FRONT = 'F', BACK  = 'B' 
};

struct seat
{
    int row, col;
    int id;
};

struct seat get_seat(const char* bpass)
{
    int rowMin = 0, rowMax = 127;
    int colMin = 0, colMax = 7;

    for(int i=0;bpass[i]!='\0';i++)
    {
        switch (bpass[i])
        {
        case LEFT:
            colMax = colMin + (colMax - colMin) / 2;
            break;
        case RIGHT:
            colMin = colMin + (colMax - colMin) / 2;
            break;
        case FRONT:
            rowMax = rowMin + (rowMax - rowMin) / 2;
            break;
        case BACK:
            rowMin = rowMin + (rowMax - rowMin) / 2;
            break;
        }
    }

    return (struct seat){rowMax, colMax, rowMax * 8 + colMax};
}

int main(void)
{
    DAY(5, "Binary Boarding")

    int64_t max_seat_id = -1;

    uint32_t used_seats[128*8/32] = { 0 };

    while (peekchar()!=EOF)
    {
        char bpass[11] = {'\0'};
        scanf_s("%s\n", bpass, sizeof(bpass));

        struct seat seat = get_seat(bpass);
        
        bit_array_set(used_seats, seat.id);
        max_seat_id = max(max_seat_id, seat.id);
    }

    int64_t my_seat_id = 1;
    for (;;my_seat_id++)
    {
        if (bit_array_is_set(used_seats, my_seat_id-1) &&
            !bit_array_is_set(used_seats, my_seat_id) &&
            bit_array_is_set(used_seats, my_seat_id+1))
            break;
    }


    SOLUTION(max_seat_id, my_seat_id)
}