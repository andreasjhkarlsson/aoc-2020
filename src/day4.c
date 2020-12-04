#include "adventofcode.h"
#include "bitarray.h"
#include "util.h"
#include <stdbool.h>
#include <stdarg.h>

enum PassportField
{
    IYR = 0, CID = 1,
    ECL = 2, HCL = 3,
    BYR = 4, PID = 5,
    EYR = 6, HGT = 7
};

// Maps the keys in the puzzle to above enum
uint32_t hash(const char* k)
{
    char k0 = (k[0] - 'a') >> 1;
    k0 = (((k0&1) & ((k0>>2) & 1)) << 1) ^ k0;

    char k1 = (k[1] - 'a') >> 2;
    k1 = k1 >> (k1 >> 2);

    return (k0 + (k1 << 2)) % 8;
}

bool all_digits(const char* str)
{
    for(int i=0;str[i]!='\0';i++)
    {
        if (!(str[i] >= '0' && str[i]<='9'))
            return false;
    }
    return true;
}

bool validate_field(char field[3], const char* value)
{
    switch (hash(field))
    {
    case BYR:
    {
        int byr = atoi(value);
        return byr >= 1920 && byr <= 2002;
    }
    case IYR:
    {
        int iyr = atoi(value);
        return iyr >= 2010 && iyr <= 2020;

    }
    case EYR:
    {
        int eyr = atoi(value);
        return eyr >= 2020 && eyr <= 2030;
    }
    case HGT:
    {
        int min = 0, max = 0;
        int hgt = atoi(value);
        switch (value[strlen(value)-1])
        {
            case 'm':
                min = 150;
                max = 193;
                break;
            case 'n':
                min = 59;
                max = 76;
                break;
            default:
                return false;
        }

        return hgt >= min && hgt <= max;
    }
    case HCL:
    {
        if (value[0]!='#')
            return false;

        if (strlen(value)!=7)
            return false;
        for(int i=1;value[i]!='\0';i++)
        {
            if (!((value[i] >= '0' && value[i] <= '9') || (value[i] >= 'a' && value[i] <= 'f')))
                return false;
        }

        return true;
    }
    case ECL:
        return matches_any(value, 7, "amb", "blu", "brn", "gry", "grn", "hzl", "oth");
    case PID:
    {
        return strlen(value) == 9 && all_digits(value);
    }
    default:
        return true;

    }
}

int main(void)
{
    DAY(4, "Passport Processing")

    int64_t part1 = 0;
    int64_t part2 = 0;

    while (peekchar()!=EOF)
    {
        uint8_t passport = 0;
        bool valid = true;
        while (peekchar()!='\n' && peekchar()!=EOF)
        {
            char key[3];
            char value[128];
            scanf("%c%c%c:%s", &key[0],&key[1],&key[2], value);

            valid &= validate_field(key, value);
            
            skipchar();
            passport |= 1 << hash(key);
        }
        skipchar();

        passport |= 1 << CID;

        if (passport == 0xFF)
        {    
            part1++;
            if (valid)
                part2++;
        }
    }

    SOLUTION(part1, part2)
}