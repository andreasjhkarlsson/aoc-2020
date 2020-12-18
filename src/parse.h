#ifndef PARSE_H
#define PARSE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

struct parse_result
{
    bool success;
    union
    {
        intptr_t result;
        intptr_t results[16];
    };
    
    int count;
    const char* rest;
};

typedef struct parse_result (*parser)(const char*);

struct parse_result parse_failed(const char *str);

struct parse_result parse_succeeded(intptr_t result, const char* rest);

struct parse_result parse_char(const char* str, char c);

struct parse_result parse_digit(const char* str);

struct parse_result parse_int(const char* str);

struct parse_result parse_terminator(const char* str);

// parse either a or b
struct parse_result parse_either(const char* str, parser a, parser b);

// Parse all passed parsers in sequence
struct parse_result parse_sequence(const char* str, int count, ...);

#define CHAR_PARSER(NAME, CHAR) struct parse_result NAME(const char* str) { return parse_char(str, CHAR); }

#endif