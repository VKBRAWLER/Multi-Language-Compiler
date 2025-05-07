#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct ASTNode {
    char value[50];
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode *parse(Token tokens[], int token_count);

#endif