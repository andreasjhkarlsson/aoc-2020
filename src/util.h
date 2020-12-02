#ifndef UTIL_H
#define UTIL_H
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

int atoi_s(const char * str, size_t len);

int peekchar();

int getint();

void skipchar();

#endif