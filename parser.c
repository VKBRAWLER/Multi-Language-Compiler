#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int isOperator(char *token) {
    return (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
            strcmp(token, "=") == 0);
}

ASTNode *createNode(const char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    strcpy(node->value, value);
    node->left = node->right = NULL;
    return node;
}

ASTNode *buildASTFromPostfix(char *postfix[], int size) {
    ASTNode *stack[100];
    int top = -1;

    for (int i = 0; i < size; i++) {
        if (isOperator(postfix[i])) {
            ASTNode *node = createNode(postfix[i]);
            node->right = stack[top--];
            node->left = stack[top--];
            stack[++top] = node;
        } else {
            stack[++top] = createNode(postfix[i]);
        }
    }

    return stack[top];
}

int toPostfix(Token tokens[], int token_count, char *postfix[]) {
    char *stack[100];
    int top = -1, postfix_index = 0;

    for (int i = 0; i < token_count; i++) {
        if (!isOperator(tokens[i].value) && strcmp(tokens[i].value, "(") != 0 && strcmp(tokens[i].value, ")") != 0) {
            postfix[postfix_index++] = tokens[i].value;
        } else if (strcmp(tokens[i].value, "(") == 0) {
            stack[++top] = tokens[i].value;
        } else if (strcmp(tokens[i].value, ")") == 0) {
            while (top >= 0 && strcmp(stack[top], "(") != 0) {
                postfix[postfix_index++] = stack[top--];
            }
            top--; // Pop '('
        } else {
            while (top >= 0 && precedence(stack[top][0]) >= precedence(tokens[i].value[0])) {
                postfix[postfix_index++] = stack[top--];
            }
            stack[++top] = tokens[i].value;
        }
    }

    while (top >= 0) {
        postfix[postfix_index++] = stack[top--];
    }

    return postfix_index;
}

ASTNode *parse(Token tokens[], int token_count) {
    char *postfix[100];
    int postfix_size = toPostfix(tokens, token_count, postfix);
    return buildASTFromPostfix(postfix, postfix_size);
}