#include "adventofcode.h"
#include "util.h"
#include <stdbool.h>

struct requirement
{
    int amount;
    char color[32];
};

struct rule
{
    char color[32];
    int requirement_count;
    struct requirement requirements[8];
};

void read_color(char* color)
{
    scanf("%s ", color);
    size_t len = strlen(color);
    color[len] = ' ';
    scanf("%s", color + len + 1);
}

void read_requirement(struct requirement* req)
{
    if (peekchar()==' ')
        skipchar();

    scanf("%d ", &req->amount);
    read_color(req->color);
    scanf(" bag");
    if (req->amount > 1) // plural s
        skipchar();
};

void read_rule(struct rule* rule)
{
    rule->requirement_count = 0;
    read_color(rule->color);
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

struct rule* find_rule(const char* name, struct rule rules[], int rule_count)
{
    for (int i=0;i<rule_count;i++)
    {
        if (strcmp(name, rules[i].color)==0)
            return &rules[i];
    }
    return NULL;
}

bool can_bag_contain(struct rule* rule, const char* target, struct rule rules[], int rule_count)
{
    for(int i=0;i<rule->requirement_count;i++)
    {
        if (strcmp(rule->requirements[i].color, target) == 0)
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
        if (can_bag_contain(&rules[i], "shiny gold", rules, rule_count))
            part1++;
    }
    
    int64_t part2 = bags_required(find_rule("shiny gold", rules, rule_count), rules, rule_count);

    SOLUTION(part1, part2)
}