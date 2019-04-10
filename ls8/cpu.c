#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

void handle_LDI(struct cpu *cpu)
{
  /* LDI reg int */
  printf("load %u into reg %u\n",
         cpu_ram_read(cpu, cpu->pc + 1),
         cpu_ram_read(cpu, cpu->pc));

  cpu_register_write(cpu,
                     cpu_ram_read(cpu, cpu->pc),
                     cpu_ram_read(cpu, cpu->pc + 1));
  // cpu->reg[cpu->ram[cpu->pc]] = cpu->ram[cpu->pc + 1];
  cpu->pc += 2;
}

void handle_MUL(struct cpu *cpu)
{
  alu(cpu, ALU_MUL, cpu_ram_read(cpu, cpu->pc), cpu_ram_read(cpu, cpu->pc + 1));
  cpu->pc += 2;
}

void handle_HLT(int *running)
{
  *running = 0;
  printf("END PROGRAM\n");
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filepath)
{
  int address = 0;
  FILE *fp = fopen(filepath, "r");

  if (!fp)
  {
    perror("File opening failed");
    // return 2;
  }

  char buf[1024];
  // read file loop
  // char *c;
  // unsigned char *ramp = cpu->ram;
  while (fgets(buf, 1024, fp) != NULL)
  {
    char *endptr;
    // *ramp = c;

    unsigned char val = strtoul(buf, &endptr, 2);

    printf("line: %_2u, loading ram: %_4u\n", address, val);

    if (buf == endptr)
    {
      continue;
    }
    // putchar(c);

    cpu_ram_write(cpu, address++, val);
    // cpu->ram[address] = val;
    // address++;
    // ramp++;
  }

  // keeps track of where the program ends
  cpu->PROGRAM_SIZE = address;

  if (ferror(fp))
    puts("I/O error when reading");
  else if (feof(fp))
    puts("End of file reached successfully");

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char result = 0;
  switch (op)
  {
  case ALU_MUL:
    printf("multipling %02u and %02u \n",
           cpu_register_read(cpu, regA),
           cpu_register_read(cpu, regA));

    result = cpu_register_read(cpu, regA) * cpu_register_read(cpu, regB);
    cpu_register_write(cpu, regA, result);
    break;
    // TODO: implement more ALU ops
  }
}

void cpu_print_state(struct cpu *cpu)
{
  printf("\ncpu->pc: 0x%02x op: %_3u, registers: [ ", cpu->pc, cpu->ram[cpu->pc]);
  for (int i = 0; i < 8; i++)
  {
    printf("%3u ", cpu->reg[i]);
  }
  printf("]\n");
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char current_instruction;
  unsigned int num_err = 0;
  unsigned char reg_0, reg_1, reg_2, val;

  while (running)
  {

#ifdef DEBUG
    cpu_print_state(cpu);
#endif
    if (num_err > 5)
    {
      printf("too many bad instructions: %u. Ending emulation\n", num_err);
      break;
    }

    // TODO
    // 1. Get the value of the current instruction (in address PC).
    current_instruction = cpu_ram_read(cpu, cpu->pc++);
    // 2. Figure out how many operands this next instruction requires
    // unsigned int num_ops =
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    switch (current_instruction)
    {
    case HLT:
      handle_HLT(&running);
      break;

    case PRN:
      handle_PRN(cpu);
      break;

    case LDI:
      handle_LDI(cpu);
      break;

    case MUL:
      handle_MUL(cpu);
      break;

    case POP:
      val = cpu_pop_stack(cpu);
      // cpu_register_write(cpu, cpu_ram_read(cpu, cpu->pc), val);
      break;

    case PUSH:
      // val = cpu_register_read(cpu, cpu_ram_read(cpu, cpu->pc));
      cpu_push_stack(cpu);
      // printf("pushing %u onto the stack from register %u\n", val, cpu_ram_read(cpu, cpu->pc));
      break;

    default:
      printf("Invalid instruction: %u\n", current_instruction);
      num_err++;
      cpu->pc++;
      break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    // cpu->pc++;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // cpu->ram;
  cpu->pc = 0;
  cpu->reg[7] = MEM_SIZE - 12; // this is where the stack pointer is supposed to live
  cpu->reg[7] = MEM_SIZE - 12; // SP_START;
}

// helper functions
unsigned char cpu_ram_read(struct cpu *cpu, unsigned int ram_index)
{
  return cpu->ram[ram_index];
}

void cpu_ram_write(struct cpu *cpu, unsigned int ram_index, unsigned char value)
{
  cpu->ram[ram_index] = value;
}

unsigned char cpu_register_read(struct cpu *cpu, unsigned int reg_i)
{
  return cpu->reg[reg_i];
}

void cpu_register_write(struct cpu *cpu, unsigned int reg_i, unsigned char value)
{
  cpu->reg[reg_i] = value;
}

// operation handlers
void handle_PRN(struct cpu *cpu)
{
  // unsigned int value = cpu->ram[cpu->pc++];
  printf("reg: %u ", cpu_ram_read(cpu, cpu->pc));
  printf("\"%u\"", cpu_register_read(cpu,
                                     cpu_ram_read(cpu, cpu->pc)));
  cpu->pc++;
}

void cpu_push_stack(struct cpu *cpu)
{
  unsigned char val;
  val = cpu_register_read(cpu, cpu_ram_read(cpu, cpu->pc));
  if (cpu->reg[7] >= cpu->PROGRAM_SIZE)
  {
    cpu->reg[7]--; // move to next empty stack location
    cpu_ram_write(cpu, cpu->reg[7], val);
  }

  cpu->pc += 1;
}

unsigned char cpu_pop_stack(struct cpu *cpu)
{
  unsigned char popped_value = cpu_ram_read(cpu, cpu->reg[7]);
  cpu_register_write(cpu, cpu_ram_read(cpu, cpu->pc), popped_value);
  // checks so we can't pop past the sp start pos
  if (cpu->reg[7] <= SP_START)
  {
    cpu->reg[7]++; // move sp to previous value on stack
  }
  printf("popping %u off the stack into register %u\n", popped_value, cpu_ram_read(cpu, cpu->pc));
  cpu->pc += 1;
  return popped_value;
}