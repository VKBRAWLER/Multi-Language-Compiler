#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for IR instructions
typedef struct {
  char opcode[10];
  char operand1[10];
  char operand2[10];
  char result[10];
} IRInstruction;

// Function to generate an IR instruction
void generateIR(IRInstruction *ir, const char *opcode, const char *operand1, const char *operand2, const char *result) {
  strncpy(ir->opcode, opcode, sizeof(ir->opcode) - 1);
  strncpy(ir->operand1, operand1, sizeof(ir->operand1) - 1);
  strncpy(ir->operand2, operand2, sizeof(ir->operand2) - 1);
  strncpy(ir->result, result, sizeof(ir->result) - 1);
}

// Function to print an IR instruction
void printIR(const IRInstruction *ir) {
  printf("%s %s, %s -> %s\n", ir->opcode, ir->operand1, ir->operand2, ir->result);
}

int main() {
  // Example: Generate and print some IR instructions
  IRInstruction ir[3];

  generateIR(&ir[0], "ADD", "R1", "R2", "R3");
  generateIR(&ir[1], "SUB", "R3", "R4", "R5");
  generateIR(&ir[2], "MUL", "R5", "R6", "R7");

  printf("Generated IR Instructions:\n");
  for (int i = 0; i < 3; i++) {
    printIR(&ir[i]);
  }

  return 0;
}