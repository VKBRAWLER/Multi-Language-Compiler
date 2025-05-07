#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "ir_generator.h"
#include "assembly_generator.h"

int main() {
    const char *source_code = "a = 4 + 2 * 10"; // Example input

    // Step 1: Lexical Analysis
    Token tokens[100];
    int token_count = tokenize(source_code, tokens);

    // Step 2: Syntax Analysis (Parsing)
    ASTNode *ast = parse(tokens, token_count);

    // Step 3: Intermediate Representation (IR) Generation
    IRInstruction ir[100];
    int ir_count = generateIRFromAST(ast, ir);

    // Step 4: Assembly Code Generation
    generateAssembly(ir, ir_count, "output.asm");

    printf("Assembly code generated in 'output.asm'\n");

    return 0;
}