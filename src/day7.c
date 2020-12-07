#include "adventofcode.h"
#include "util.h"
#include "string_store.h"
#include <stdbool.h>

// Allocate 16 kB for strings
STRING_STORAGE(1024*16);

struct requirement
{
    int amount;
    pinned_string color;
};

struct rule
{
    pinned_string color;
    int requirement_count;
    struct requirement requirements[8];
};

pinned_string read_color()
{
    char color[32];
    scanf("%s ", color);
    size_t len = strlen(color);
    color[len] = ' ';
    scanf("%s", color + len + 1);
    return PIN_STRING(color);
}

void read_requirement(struct requirement* req)
{
    if (peekchar()==' ')
        skipchar();

    scanf("%d ", &req->amount);
    req->color = read_color();
    scanf(" bag");
    if (req->amount > 1) // plural s
        skipchar();
};

void read_rule(struct rule* rule)
{
    rule->requirement_count = 0;
    rule->color = read_color();
    scanf(" bags contain ");

    if (peekchar()=='n')
    {
        scanf("no other bags.");
        return;
    }
    
    do
    {
        read_requirement(&rule->requirements[rule->requirement_count++]);
    } while (getchar()==',');

}

struct rule* find_rule(pinned_string name, struct rule rules[], int rule_count)
{
    for (int i=0;i<rule_count;i++)
    {
        if (name==rules[i].color)
            return &rules[i];
    }
    return NULL;
}

bool can_bag_contain(struct rule* rule, pinned_string target, struct rule rules[], int rule_count)
{
    for(int i=0;i<rule->requirement_count;i++)
    {
        if (rule->requirements[i].color==target)
            return true;
        else if (can_bag_contain(find_rule(rule->requirements[i].color, rules, rule_count), target, rules, rule_count) > 0)
            return true;
    }

    return false;
}

int bags_required(struct rule* rule, struct rule rules[], int rule_count)
{
    int requires = 0;
    for (int i=0;i<rule->requirement_count;i++)
    {
        requires += rule->requirements[i].amount * (bags_required(find_rule(rule->requirements[i].color,rules, rule_count), rules, rule_count) + 1);
    }

    return requires;
}

#define SHINY_GOLD PIN_STRING("shiny gold")

int main(void)
{
    DAY(7, "Handy Haversacks")

    struct rule rules[1024];
    int rule_count = 0;

    while (peekchar()!=EOF)
    {
        read_rule(&rules[rule_count++]);
        skipchar();
    }


    int64_t part1 = 0;
    for (int i=0;i<rule_count;i++)
    {
        if (can_bag_contain(&rules[i], SHINY_GOLD, rules, rule_count))
            part1++;
    }
    
    int64_t part2 = bags_required(find_rule(SHINY_GOLD, rules, rule_count), rules, rule_count);

    SOLUTION(part1, part2)
}