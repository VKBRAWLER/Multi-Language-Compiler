#include <stdio.h>
#include "assembly_generator.h"

void generateAssembly(IRInstruction ir[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s, %s -> %s\n", ir[i].opcode, ir[i].operand1, ir[i].operand2, ir[i].result);
    }

    fclose(file);
}
