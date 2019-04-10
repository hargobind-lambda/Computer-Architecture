#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU

// cpu constants
#define MEM_SIZE 256
#define SP_START 256 - 12

struct cpu {
  // PC
  unsigned int pc;
  // Stack pointer
  unsigned int sp;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[MEM_SIZE];
  unsigned char PROGRAM_SIZE;

};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define PUSH 0b01000101
#define POP  0b01000110 

// PC mutators
#define CALL 0b01010000 
#define RET  0b00010001

#define INT  0b01010010 
#define IRET 0b00010011

#define JMP  0b01010100 
#define JEQ  0b01010101 
#define JNE  0b01010110 
#define JGT  0b01010111 
#define JLT  0b01011000 
#define JLE  0b01011001 
#define JGE  0b01011010 

// ALU ops
#define MUL  0b10100010
#define ADD  0b10100000 // 00000aaa 00000bbb
#define SUB  0b10100001 // 00000aaa 00000bbb
#define MUL  0b10100010 // 00000aaa 00000bbb
#define DIV  0b10100011 // 00000aaa 00000bbb
#define MOD  0b10100100 // 00000aaa 00000bbb

#define INC  0b01100101 // 00000rrr
#define DEC  0b01100110 // 00000rrr

#define CMP  0b10100111 // 00000aaa 00000bbb

#define AND  0b10101000 // 00000aaa 00000bbb
#define NOT  0b01101001 // 00000rrr
#define OR   0b10101010 // 00000aaa 00000bbb
#define XOR  0b10101011 // 00000aaa 00000bbb
#define SHL  0b10101100 // 00000aaa 00000bbb
#define SHR  0b10101101 // 00000aaa 00000bbb


// TODO: more instructions here. These can be used in cpu_run().

// ALU operations

enum alu_op {
  ALU_ADD,
  ALU_SUB,
  ALU_MUL,
  ALU_DIV,
  ALU_MOD,

  ALU_INC,
  ALU_DEC,

  ALU_CMP,

  ALU_AND,
  ALU_NOT,
  ALU_OR,
  ALU_XOR,
  ALU_SHL,
  ALU_SHR
};

// Function declarations

extern void cpu_load(struct cpu *cpu, char * filepath);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif