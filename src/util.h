#ifndef UTIL_H
#define UTIL_H
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

int atoi_s(const char * str, size_t len);

int peekchar();

int getint();

void skipchar();

bool matches_any(const char* str, int count, ...);

#endif