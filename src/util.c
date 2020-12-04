#include "util.h"
#include <limits.h>
#include <stdio.h>

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

int peekchar()
{
    char c = getchar();
    ungetc(c, stdin);
    return c;
}

int getint()
{
    int result = 0;

    while(result < INT_MAX)
    {
        char c = peekchar();
        if (c >='0' && c<='9')
        {
            getchar();
            result = result * 10 + (c - '0');
        }
        else
            break;
        
    }
    return result;
}

void skipchar()
{
    getchar();
}

bool matches_any(const char* str, int count, ...)
{
    va_list valist;

    va_start(valist, count);

    for(int i=0;i<count;i++)
    {
        if (strcmp(str, va_arg(valist, const char *)) == 0)
            return true;
    }
	
    va_end(valist);

    return false;
}