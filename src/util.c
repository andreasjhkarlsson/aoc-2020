#include "util.h"

int atoi_s(const char* str, size_t len)
{
    int res = 0;
    for(size_t i=0; i<len; i++)
    {
        char c = str[i];
        if(c>='0' && c<='9')
            res = res * 10 + (c - '0');
        else
            return res;
    }

    return res;
}