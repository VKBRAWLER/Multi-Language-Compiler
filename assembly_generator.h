#ifndef ASSEMBLY_GENERATOR_H
#define ASSEMBLY_GENERATOR_H

#include "ir_generator.h"

void generateAssembly(IRInstruction ir[], int count, const char *filename);

#endif