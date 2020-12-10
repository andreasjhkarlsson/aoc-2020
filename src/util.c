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

#define SWAP(A,B) { A ^=  B; B ^= A; A ^= B; }

void sqsort(int list[], int length)
{
    switch (length)
    {
    case 0:
    case 1:
        return; // Already sorted
    case 2:
        if (list[0] > list[1])
            SWAP(list[0], list[1])
        return;
    }

    // VLA <3
    int sublist[length];
    int pivot = list[0];    
    int li=0, gi = length-1; 

    for (int i=1;i<length;i++)
    {
        if (list[i] <= pivot)
            sublist[li++] = list[i];
        else
            sublist[gi--] = list[i];
    }

    sqsort(sublist, li);
    sqsort(&sublist[gi+1], length-gi-1);
    sublist[gi] = pivot;

    memcpy(list, sublist, length * sizeof(int));   
}

void memset64(int64_t* dst, int64_t val, size_t size)
{
    for(size_t i=0; i<size; i++)
        dst[i] = val;
}