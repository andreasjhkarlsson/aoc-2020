#include "adventofcode.h"
#include "util.h"
#include "parse.h"

CHAR_PARSER(parse_ws, ' ')
CHAR_PARSER(parse_op_plus, '+')
CHAR_PARSER(parse_op_mul, '*')
CHAR_PARSER(parse_left_paren, '(')
CHAR_PARSER(parse_right_paren, ')')

struct parse_result parse_expr1(const char* str);

struct parse_result parse_group1(const char* str)
{
    struct parse_result res = parse_sequence(str, 3, &parse_left_paren, &parse_expr1, &parse_right_paren);
    if (!res.success)
        return res;
   
    return parse_succeeded(res.results[1],res.rest);
}

struct parse_result parse_operand(const char* str)
{
    return parse_either(str, &parse_group1, &parse_int);
}

struct parse_result parse_operator(const char* str)
{
    return parse_either(str, &parse_op_plus, &parse_op_mul);
}

struct parse_result parse_part(const char* str, intptr_t lhs)
{

    struct parse_result res = parse_terminator(str);
    if (res.success)
        return parse_succeeded(lhs, str);

    res = parse_sequence(str, 4, &parse_ws, &parse_operator, &parse_ws, &parse_operand);

    if (!res.success)
        return res;

    intptr_t rhs = res.results[3];
    intptr_t value;

    switch (res.results[1])
    {
    case '+':
        value = lhs + rhs;
        break;
    case '*':
        value = lhs * rhs;
        break;
    default:
        BAIL("Unsupported operator")
    }

    res = parse_part(res.rest, value);

    if (res.success)
        return res;
        
    return parse_succeeded(value, res.rest);
}

struct parse_result parse_expr1(const char* str)
{
    struct parse_result res = parse_operand(str);
    if (!res.success)
        return res;
    return parse_part(res.rest, res.result);
}

int64_t eval1(const char* str)
{
    /*
        Part 1 grammar:
        expr ::= operand part
        part ::= (+|*) operand part | ε
        operand ::= ( expr ) | integer
    */
    return parse_expr1(str).result;
}

struct parse_result parse_expr2(const char* str);
struct parse_result parse_factor(const char* str);
struct parse_result parse_term(const char* str);

struct parse_result parse_multiplication(const char* str)
{
    struct parse_result res = parse_sequence(str, 5, &parse_factor, &parse_ws, &parse_op_mul, &parse_ws, &parse_expr2);

    if (res.success)
        return parse_succeeded(res.results[0] * res.results[4], res.rest);
    
    return parse_failed(str);
}

struct parse_result parse_addition(const char* str)
{
    struct parse_result res = parse_sequence(str, 5, &parse_term, &parse_ws, &parse_op_plus, &parse_ws, &parse_factor);

    if (res.success)
        return parse_succeeded(res.results[0] + res.results[4], res.rest);
    
    return parse_failed(str);
}

struct parse_result parse_group2(const char* str)
{
    struct parse_result res = parse_sequence(str, 3, &parse_left_paren, &parse_expr2, &parse_right_paren);
    if (res.success)
        return parse_succeeded(res.results[1], res.rest);
    return parse_failed(str);
}

struct parse_result parse_factor(const char* str)
{
    return parse_either(str, &parse_addition, &parse_term);
}

struct parse_result parse_term(const char* str)
{
    return parse_either(str, &parse_group2, &parse_int);
}

struct parse_result parse_expr2(const char* str)
{
    return parse_either(str, &parse_multiplication, &parse_factor);
}

int64_t eval2(const char* str)
{
    /*
        Part 2 grammar:
        expr2 ::= factor * expr | factor
        factor ::= term + factor | term
        term ::= (expr2) | int
    */
    return parse_expr2(str).result;
}

int main(void)
{
    DAY(18, "Operation Order")

    char line[256];

    int64_t part1 = 0, part2 = 0;
    while (peekchar()!=EOF)
    {
        _getline(line, sizeof(line));
        
        part1 += eval1(line);
        part2 += eval2(line);
    }

    SOLUTION(part1, part2)
}


