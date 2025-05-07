#ifndef LEXER_H
#define LEXER_H

typedef struct {
    char value[50];
} Token;

int tokenize(const char *source, Token tokens[]);

#endif