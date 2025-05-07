#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "parser.h"

typedef struct {
    char opcode[10];
    char operand1[10];
    char operand2[10];
    char result[10];
} IRInstruction;

int generateIRFromAST(ASTNode *node, IRInstruction ir[]);

#endif