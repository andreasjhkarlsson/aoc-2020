#include "adventofcode.h"
#include "util.h"
#include <stdbool.h>

bool is_valid_password(const char* password, char required_char, int min, int max)
{
    int occurrences = 0;
    for(int i=0;password[i]!='\0';i++)
    {
        if (password[i]==required_char)
            occurrences++;
    } 
    return occurrences >= min && occurrences <= max;
}

bool is_valid_password_v2(const char* password, char required_char, int min, int max)
{
    return (password[min-1] == required_char) ^ (password[max-1]==required_char);
}

int main()
{
    DAY(2, "Password Philosophy")

    int64_t part1 = 0;
    int64_t part2 = 0;
    while (peekchar()!=EOF)
    {
        int min, max;
        char requirement;
        char password[128];
        scanf_s("%d-%d %c: %s\n", &min, &max, &requirement, 1, password, sizeof(password));
        
        if (is_valid_password(password, requirement, min, max))
            part1++;
        if (is_valid_password_v2(password, requirement, min, max))
            part2++;
    }

    SOLUTION(part1, part2)
}