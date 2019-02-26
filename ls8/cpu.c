#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**  Read from the CPU's ram at the specifc location 
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

/**  Writes at index specified for the CPU's ram
 */
void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char num)
{
  cpu->ram[MAR] = num;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv)
{
  FILE *ptr = fopen(argv, "r");
  char c[100];
  long ret;

  int address = 0;
  while (fgets(c, 100, ptr) != NULL)
  {
    if (c[0] == '1' || c[0] == '0')
    {
      ret = strtoul(c, NULL, 2);
      cpu->ram[address++] = ret;
    }
  }

  fclose(ptr);

  // Pevious hardcode
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  (void)cpu;
  (void)regA;
  (void)regB;
  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{

  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, (cpu->pc));
    unsigned char operandA = cpu_ram_read(cpu, (cpu->pc + 1));
    unsigned char operandB = cpu_ram_read(cpu, (cpu->pc + 2));

    switch (IR)
    {
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;
    case HLT:
      running = 0;
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    default:
      printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->pc);
      exit(1);
    }

    int opNum = (11000000 & IR) >> 6;
    cpu->pc += (opNum + 1);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->registers[7] = 0xF4;
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
