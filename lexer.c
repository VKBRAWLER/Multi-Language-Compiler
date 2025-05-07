#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

int tokenize(const char *source, Token tokens[]) {
    int count = 0;
    int i = 0;

    while (source[i] != '\0') {
        if (isspace(source[i])) {
            i++;
            continue;
        }

        if (isdigit(source[i]) || isalpha(source[i])) {
            int j = 0;
            while (isdigit(source[i]) || isalpha(source[i])) {
                tokens[count].value[j++] = source[i++];
            }
            tokens[count].value[j] = '\0';
            count++;
        } else if (strchr("+-*/=()", source[i])) {
            tokens[count].value[0] = source[i];
            tokens[count].value[1] = '\0';
            count++;
            i++;
        } else {
            printf("Error: Unknown character '%c'\n", source[i]);
            return -1;
        }
    }

    return count;
}