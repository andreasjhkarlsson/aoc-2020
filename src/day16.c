#include "adventofcode.h"
#include "util.h"

struct rule
{
    char name[32];
    struct range
    {
        int from, to;
    } inclusion_ranges[2];
};

struct ticket
{
    int fields[32];
    int fields_length;
};

void read_rule(struct rule* rule)
{
    int name_length=0;
    while (peekchar()!=':')
        rule->name[name_length++] = getchar();
    
    rule->name[name_length] = '\0';

    scanf(": %d-%d or %d-%d",
        &rule->inclusion_ranges[0].from,
        &rule->inclusion_ranges[0].to,
        &rule->inclusion_ranges[1].from,
        &rule->inclusion_ranges[1].to
    );
    skipchar(); // \n
}

void read_ticket(struct ticket* ticket)
{
    ticket->fields_length = 0;
    
    while (peekchar()!='\n')
        scanf("%d,", &ticket->fields[ticket->fields_length++]);
        
    skipchar(); // \n    
}

bool matches_rule(struct rule* rule, int num)
{
    for(int j=0;j<2;j++)
    {
        if (num >= rule->inclusion_ranges[j].from && num <= rule->inclusion_ranges[j].to)
            return true;
    }
    return false;
}

bool matches_any_rule(struct rule* rules, int rules_length, int num)
{
    for(int i=0;i<rules_length;i++)
    {
        if (matches_rule(&rules[i], num))
            return true;
    }
    return false;
}


bool resolve_fields(struct rule rules[], int rules_length, struct ticket tickets[], int tickets_length, struct rule* matches[], int matches_length)
{
    int to_resolve = -1;
    for(int i=0;i<matches_length;i++)
    {
        if (matches[i]==NULL)
        {
            to_resolve = i;
            break;
        }
    }

    if (to_resolve==-1)
        return true;

    for(int ri=0;ri<rules_length;ri++)
    {
        struct rule* rule = &rules[ri];
        for (int mi=0;mi<matches_length;mi++)
        {
            if (matches[mi] == rule)
                goto next_rule;
        }
        
        for(int ti=0;ti<tickets_length;ti++)
        {
            if (!matches_rule(rule, tickets[ti].fields[to_resolve]))
                goto next_rule;
        }

        matches[to_resolve] = rule;
        if (resolve_fields(rules, rules_length, tickets, tickets_length, matches, matches_length))
            return true;
        matches[to_resolve] = NULL;

        next_rule:;
    } 
    return false;
}

int main(void)
{
    DAY(16, "Ticket Translation")

    struct rule rules[128];
    int rules_length = 0;

    while (peekchar()!=EOF && peekchar()!='\n')
        read_rule(&rules[rules_length++]);

    scanf("\nyour ticket:\n");

    struct ticket tickets[512];
    int tickets_length = 0;
    read_ticket(&tickets[tickets_length++]);

    scanf("\nnearby tickets:\n");

    while (peekchar()!=EOF)
        read_ticket(&tickets[tickets_length++]);

    struct ticket valid_tickets[512];
    int valid_tickets_length = 0;

    int64_t part1 = 0LL;
    for (int i=1;i<tickets_length;i++)
    {
        struct ticket ticket = tickets[i];
        bool valid = true;
        for (int j=0;j<ticket.fields_length;j++)
        {
            int num = ticket.fields[j];
            if (!matches_any_rule(rules, rules_length, num))
            {
                part1 += num;
                valid = false;
            }
        }

        if (valid)
            valid_tickets[valid_tickets_length++] = ticket;
    }

    struct rule* matches[128] = {0};
    resolve_fields(rules, rules_length, valid_tickets, valid_tickets_length, matches, rules_length);

    int64_t part2 = 1LL;
    for (int i=0;i<rules_length;i++)
    {
        struct rule* rule = matches[i];
        if (starts_with(rule->name, "departure"))
            part2 *= tickets[0].fields[i];
    }

    SOLUTION(part1,part2)
}