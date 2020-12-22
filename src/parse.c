#include "parse.h"


struct parse_result parse_failed(const char *str)
{
    return (struct parse_result) { .success = false, .rest = str };
}

struct parse_result parse_succeeded(intptr_t result, const char* rest)
{
    return (struct parse_result) { .success = true, .result = result, .count = 1, .rest = rest};
}

struct parse_result parse_succeedeed_with_data(void* data, size_t size, const char* rest)
{
    struct parse_result res = { .success = true, .rest = rest };
    res.count = size;
    memcpy(&res.data[0], data, size);
    return res;
}

struct parse_result parse_char(const char* str, char c)
{
    if (*str==c)
        return parse_succeeded(c, str+1);
    return parse_failed(str);
}

struct parse_result parse_digit(const char* str)
{
    if (*str >= '0' && *str <='9')
        return parse_succeeded(*str -'0', str+1);
    return parse_failed(str);
}

struct parse_result parse_int(const char* str)
{
    struct parse_result res = parse_digit(str);

    if(res.success)
    {
        intptr_t integer = 0;
        while (res.success)
        {
            integer = integer * 10 + res.result;
            res = parse_digit(res.rest);
        }
        return parse_succeeded(integer, res.rest);
    }

    return res;
}

struct parse_result parse_terminator(const char* str)
{
    if (!*str)
        return parse_succeeded('\0', str);
    return parse_failed(str);
}


// parse either a or b
struct parse_result parse_either(const char* str, parser a, parser b)
{
    struct parse_result res = a(str);
    if (res.success)
        return res;
    return b(str);
}

struct parse_result parse_sequence(const char* str, int count, ...)
{
    va_list valist;

    va_start(valist, count);

    struct parse_result res = {.success = true, .count = 0, .rest = str};

    for(int i=0;i<count;i++)
    {
        parser p = va_arg(valist, parser);
        struct parse_result pr = p(res.rest);
        if (!pr.success)
        {
            res = parse_failed(str);
            goto end;
        }
        res.results[res.count++] = pr.result;
        res.rest = pr.rest;
    }
    
    end: va_end(valist);

    return res;
}

struct parse_result parse_map_result(struct parse_result parse_result, int index)
{
    parse_result.result = parse_result.results[index];
    return parse_result;
}


struct parse_result parse_any_char(const char* str)
{
    if (*str)
        return parse_succeeded(*str, str+1);
    return parse_failed(str);
}

struct parse_result parse_string_literal(const char* str, const char* to_match)
{
    int len1 = strlen(str);
    int len2 = strlen(to_match);
    if (len2 > len1)
        return parse_failed(str);
    
    if (memcmp(str, to_match, len2)==0)
        return parse_succeeded((intptr_t)to_match, str+len2);

    return parse_failed(str);
}