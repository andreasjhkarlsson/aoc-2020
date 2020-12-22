#include "adventofcode.h"
#include "util.h"
#include "parse.h"

struct rule_list
{
    uint8_t rules[7];
    char length;
};

struct options
{
    struct rule_list rules[2];
    int length;
};

struct rule
{
    int label;
    union data
    {
        char terminal;
        struct options options;
    } data;

    enum tag
    {
        invalid,
        terminal,
        options
    } tag;
};

STRING_PARSER(parse_colon, ": ")
CHAR_PARSER(parse_quote, '"');
STRING_PARSER(parse_or, " | ")
CHAR_PARSER(parse_ws, ' ')

struct parse_result parse_terminal(const char* str)
{
    return parse_map_result(
        parse_sequence(str, 3, &parse_quote, &parse_any_char, &parse_quote),
        1
    );
}

struct parse_result parse_rule_list(const char* str)
{
    struct parse_result res = parse_int(str);
    CHECK_PARSE_RESULT(res)
    struct rule_list list;
    list.length = 0;
    while (res.success)
    {
        list.rules[list.length++] = res.result;
        res = parse_map_result( parse_sequence(res.rest, 2, &parse_ws, &parse_int), 1);
    }

    return parse_succeedeed_with_data(&list, sizeof(struct rule_list), res.rest);
}

struct parse_result parse_options(const char* str)
{
    struct parse_result res;
    struct options options;

    if((res = parse_rule_list(str)).success)
    {
        options.length = 1;
        memcpy(&options.rules[0], &res.data[0], sizeof(struct rule_list));

        if ((res = parse_or(res.rest)).success)
        {
            if (!((res = parse_rule_list(res.rest)).success))
                return parse_failed(str);
            options.length = 2;
            memcpy(&options.rules[1], &res.data[0], sizeof(struct rule_list));
        }
        
        return parse_succeedeed_with_data(&options, sizeof(struct options), res.rest);
    }
    else
        return parse_failed(str);    
}

struct parse_result parse_rule(const char* str)
{
    /*
        <rule> ::= int: "<char>" | <subrules>
        <subrules> ::= <options> | <rule_list>
        <options> ::= <rule_list> ' | ' <rule_list>
        <rule_list> ::= <int> <rule_list> | <int>
    */
    struct parse_result res = parse_sequence(str, 2, &parse_int, &parse_colon);

    CHECK_PARSE_RESULT(res)

    struct rule rule;
    rule.label = res.result;

    if ((res = parse_terminal(res.rest)).success)
    {
        rule.tag = terminal;
        rule.data.terminal = res.result;
    }
    else if ((res = parse_options(res.rest)).success)
    {
        rule.tag = options;
        memcpy(&rule.data.options, res.data, sizeof(struct options));
    }
    else
        return parse_failed(str);

    return parse_succeedeed_with_data(&rule, sizeof(struct rule), res.rest);
}

struct candidate
{
    const char* rest;
    struct candidate* next;

} candidate_store[4 * 1024 * 1024];

struct candidate* create_candidate(const char* rest, struct candidate* next)
{
    static int used = 0;
    struct candidate* c = &candidate_store[used++];
    c->rest = rest;
    c->next = next;
    return c;
}

struct candidate* link_candidates(struct candidate* a, struct candidate* b)
{
    while (a->next)
        a = a->next;

    a->next = b;
    return a;
}

struct candidate* get_rule_candidates(struct rule rules[], int label, const char* str);

struct candidate* get_rule_list_candidates(struct rule rules[], uint8_t* labels, int count, const char* str)
{
    struct candidate* cand = create_candidate(str, NULL);
    for (int i=0;i<count;i++)
    {

        struct candidate* new_candidates = NULL;
        while (cand)
        {
            struct candidate* tmp = get_rule_candidates(rules, labels[i], cand->rest);
            if (tmp)
            {
                if (new_candidates==NULL)
                    new_candidates = tmp;
                else
                    link_candidates(tmp, new_candidates);
            }

            cand = cand->next;
        }

        cand = new_candidates;
    }
    
    return cand;
}

struct candidate* get_rule_candidates(struct rule rules[], int label, const char* str)
{
    struct rule* rule = &rules[label];
    if( rule->tag == terminal)
    {
        if (rule->data.terminal == *str)
            return create_candidate(str+1, NULL);
        else
            return NULL;
    }
    struct candidate* c1 = get_rule_list_candidates(rules, rule->data.options.rules[0].rules, rule->data.options.rules[0].length, str);
    struct candidate* c2 = NULL;
    if (rule->data.options.length == 2)
    {
        c2 = get_rule_list_candidates(rules, rule->data.options.rules[1].rules, rule->data.options.rules[1].length, str);
        if (c1==NULL)
            return c2;
        else
            return link_candidates(c1,c2);
    }
    else
        return c1;

}

bool validate_rule(struct rule rules[], const char* str)
{
    struct candidate* cand = get_rule_candidates(rules, 0, str);
    while(cand)
    {
        if (!*cand->rest)
            return true;
        cand = cand->next;
    }
    return false;
}

int main(void)
{
    DAY(19, "Monster Messages")

    static struct rule rules[256];

    static char line[256];

    while (peekchar()!=EOF)
    {
        _getline(line, sizeof(line));

        struct parse_result res = parse_rule(line);
        
        if (!res.success)
            break;

        struct rule* rule =((struct rule*) res.data);
        rules[rule->label] = *rule;
    }

    static struct rule rules2[256];
    memcpy(rules2, rules, sizeof(rules));
    rules2[11] = *(struct rule*)parse_rule("11: 42 31 | 42 11 31").data;
    rules2[8] = *(struct rule*)parse_rule("8: 42 | 42 8").data;

    int64_t part1 = 0, part2 = 0;
    while (peekchar()!=EOF)
    {
        _getline(line, sizeof(line));
        
        if (validate_rule(rules, line))
            part1++;
        
        if (validate_rule(rules2, line))
            part2++;
    }

    SOLUTION(part1, part2)
}