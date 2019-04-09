#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned int ram_index)
{
  return cpu->ram[ram_index];
}

void cpu_ram_write(struct cpu *cpu, unsigned int ram_index, unsigned char value)
{
  cpu->ram[ram_index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char * filepath)
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

    printf("line: %u, loading ram: %04u\n", address, val);

    if (buf == endptr)
    {
      continue;
    }
    // putchar(c);
    
    // cpu_ram_write(cpu, address, val);
    cpu->ram[address] = val;
    address++;
    // ramp++;
  }

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
  switch (op)
  {
  case ALU_MUL:
    // unsigned int product = cpu->reg[regA] * cpu->reg[regB];
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;

    // TODO: implement more ALU ops
  }
}

void cpu_print_state(struct cpu *cpu)
{
  printf("\npc %03u op: %03u, registers: [ ", cpu->pc, cpu->ram[cpu->pc]);
  for (int i=0; i <8; i++) {
    printf("%02x ", cpu->reg[i]);
  }
  printf("]\n");
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  // unsigned int * program_counter = &cpu->pc;
  unsigned char current_instruction;
  unsigned char reg_0, reg_1, reg_2, val;

  while (running)
  {
    cpu_print_state(cpu);

    // TODO
    // 1. Get the value of the current instruction (in address PC).
    current_instruction = cpu->ram[cpu->pc++];
    // 2. Figure out how many operands this next instruction requires
    // unsigned int num_ops =
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    switch (current_instruction)
    {
    case HLT:
      running = 0;
      printf("END PROGRAM\n");
      break;

    case PRN:
      // unsigned int value = cpu->ram[cpu->pc++];
      printf("reg: %u ", cpu->ram[cpu->pc]);
      printf("\"%u\"", cpu->reg[cpu->ram[cpu->pc]]);
      cpu->pc++;
      break;

    case LDI:
      /* LDI reg int */
      printf("load %u into reg %u\n", cpu->ram[cpu->pc + 1], cpu->ram[cpu->pc]);
      cpu->reg[cpu->ram[cpu->pc]] = cpu->ram[cpu->pc + 1];
      cpu->pc += 2;
      break;

    case MUL:
      // unsigned char regA, regB;
      // unsigned char regA = cpu_ram_read(cpu, cpu->pc++);
      // unsigned char regB = cpu_ram_read(cpu, cpu->pc++);
      alu(cpu, MUL, cpu->ram[cpu->pc], cpu->ram[cpu->pc+1]);
      cpu->pc += 2;

    default:
      printf("Invalid instruction: %u\n", current_instruction);
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
}
