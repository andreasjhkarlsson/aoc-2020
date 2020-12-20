#ifndef PARSE_H
#define PARSE_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

struct parse_result
{
    bool success;
    union
    {
        intptr_t result;
        intptr_t results[16];
        char data[16 * sizeof(intptr_t)];
    };
    
    int count;
    const char* rest;
};

typedef struct parse_result (*parser)(const char*);

struct parse_result parse_failed(const char *str);

struct parse_result parse_succeeded(intptr_t result, const char* rest);

struct parse_result parse_succeedeed_with_data(void* data, size_t size, const char* rest);

struct parse_result parse_char(const char* str, char c);

struct parse_result parse_string_literal(const char* str, const char* to_match);

struct parse_result parse_digit(const char* str);

struct parse_result parse_int(const char* str);

struct parse_result parse_terminator(const char* str);

struct parse_result parse_any_char(const char* str);

struct parse_result parse_map_result(struct parse_result parse_result, int index);

// parse either a or b
struct parse_result parse_either(const char* str, parser a, parser b);

// Parse all passed parsers in sequence
struct parse_result parse_sequence(const char* str, int count, ...);

#define CHAR_PARSER(NAME, CHAR) struct parse_result NAME(const char* str) { return parse_char(str, CHAR); }
#define STRING_PARSER(NAME, STRING) struct parse_result NAME(const char* str) { return parse_string_literal(str, STRING); }
#define PARSE_EITHER(NAME, A, B) struct parse_result NAME(const char* str) { return parse_either(str, A, B); }

#define CHECK_PARSE_RESULT(RESULT) if (!RESULT.success) return RESULT;
 
#endif