#ifndef UTIL_H
#define UTIL_H
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

int atoi_s(const char * str, size_t len);

int peekchar();

int getint();

void skipchar();

bool matches_any(const char* str, int count, ...);

void _getline(char* line, size_t length);

// Stack based quick sort
void sqsort(int list[], int length);

void memset64(int64_t* dst, int64_t val, size_t size);

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#endif