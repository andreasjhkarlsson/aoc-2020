#include "adventofcode.h"
#include "util.h"
#include "bitarray.h"

enum instruction
{
    nop,
    acc,
    jmp
};

struct opcode
{
    enum instruction instruction;
    int operand;
};

enum exit_code
{
    no_error,
    infinite_loop_detected
};

struct machine
{
    int acc;
    int pc;
    enum exit_code exit_code;
};

void read_opcode(struct opcode* opcode)
{
    char inst_name[4];
    scanf("%s %i\n", inst_name, &opcode->operand);
    switch (inst_name[0])
    {
        case 'n':
            opcode->instruction = nop;
            break;
        case 'a':
            opcode->instruction = acc;
            break;
        case 'j':
            opcode->instruction = jmp;
            break;
    }
}

void step(struct machine* machine, struct opcode program[])
{
    struct opcode* opcode = &program[machine->pc];
    switch (opcode->instruction)
    {
    case nop:
        machine->pc++;
        break;
    case acc:
        machine->acc += opcode->operand;
        machine->pc++;
        break;
    case jmp:
        machine->pc += opcode->operand;
        break;
    default:
        printf("Invalid instruction detected\n");
        abort();
    }
}

struct machine run_program(struct opcode program[], const size_t program_length)
{
    struct machine machine = { 0, 0, no_error };
    
    uint32_t visited[program_length / 32];
    bit_array_init(visited, program_length);

    while (machine.pc < program_length)
    {
        if (bit_array_is_set(visited, machine.pc))
        {
            machine.exit_code = infinite_loop_detected;
            break;
        }

        bit_array_set(visited, machine.pc);

        step(&machine, program);
    }

    return machine;
}

int main(void)
{
    DAY(8, "Handheld Halting")

    static struct opcode program[4096];

    size_t program_length = 0;

    while (peekchar()!=EOF)
    {
        read_opcode(&program[program_length++]);
    }

    struct machine machine = run_program(program, program_length);
    
    int64_t part1 = machine.acc;
    int64_t part2 = -1LL;

    for (int i=0;i<program_length;i++)
    {
        enum instruction test_inst;
        
        switch (program[i].instruction)
        {
            case acc:
                continue;
            case nop:
                test_inst = jmp;
                break;
            case jmp:
                test_inst = nop;
                break;
        }

        enum instruction saved_inst = program[i].instruction;
        program[i].instruction = test_inst;
        machine = run_program(program, program_length);
        program[i].instruction = saved_inst;
        if (machine.exit_code == no_error)
        {
            part2 = machine.acc;
            break;
        }
    }
 
    SOLUTION(part1, part2)
}