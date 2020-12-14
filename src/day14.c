#include "adventofcode.h"
#include "util.h"
#include "bitarray.h"

struct mask
{
    char values[36];
};

// Addresses for part1 are only ~ 17 bits
static uint64_t v1_memory[1 << 17] = { 0ULL };

void v1_write(int address, struct mask mask, uint64_t value)
{
    if (address > 100000)
        BAIL("out of memory");

    v1_memory[address] = 0;
    for(uint64_t i=0;i<36;i++)
    {
        uint64_t bit = (value >> i) & 1;
        if (mask.values[i]!='X')
            bit = mask.values[i] == '1' ? 1: 0;
        
        v1_memory[address] |= bit << i;                           
    }    
}

// Addresses in part2 are actually 36 bits
// create binary tree to index space
struct bkey
{
    uint64_t value;
    struct bkey* bits[2];
} v2_memory[1<<20] = { {0} }; // 2^20 nodes should be enough

struct bkey* create_bkey()
{
    static size_t index = 0;
    if (index >= (sizeof(v2_memory) / sizeof(v2_memory[0])))
        BAIL("OUT OF MEMORY");
    return &v2_memory[index++];
}

uint64_t* v2_access(uint64_t address)
{
    static struct bkey* root = NULL;
    if (!root)
        root = create_bkey();   

    struct bkey* node = root;
    for(int i=0;i<36;i++)
    {
        int bit = (address >> i) & 1ULL;
        if (!node->bits[bit])
            node->bits[bit] = create_bkey();
        node = node->bits[bit];
    }

    return &node->value;
}

void v2_write(uint64_t address, struct mask mask, uint64_t value, int n)
{
    // All bits set, fire!
    if (n==36)
    {
        *v2_access(address) = value;
        return;
    }

    switch (mask.values[n])
    {
    case '1':
        v2_write(address | (1ULL << n), mask, value,n+1);
        return;
    case '0':
        v2_write(address, mask, value, n+1);
        return;
    case 'X':
        v2_write(address | (1ULL << n), mask, value,n+1);
        v2_write(address & ~(1ULL << n), mask, value,n+1);
        return;
    }
}

int main(void)
{
    DAY(14, "Docking Data")

    struct mask mask = {0};

    while (peekchar()!=EOF)
    {
        skipchar(); // m...
        if (peekchar()=='a') // ...ask!
        {
            char mask_str[37];
            scanf("ask = %s\n", mask_str);
            for(int i=0;i<36;i++)
            {
                mask.values[35-i] = mask_str[i];
            }
        }
        else if (peekchar()=='e') // ...em!
        {
            int address;
            uint64_t value = 0ULL;
            scanf("em[%d] = %"PRId64"\n",&address, &value);

            v1_write(address, mask, value);
            v2_write(address, mask, value, 0);

        }
        else BAIL("Invalid input");
    }

    uint64_t part1 = 0;
    for(int i=0;i<sizeof(v1_memory)/sizeof(v1_memory[0]);i++)
        part1 += v1_memory[i];

    uint64_t part2 = 0;
    for(int i=1;i<sizeof(v2_memory)/sizeof(v2_memory[0]);i++)
        part2 += v2_memory[i].value;

    SOLUTION(part1, part2)
}