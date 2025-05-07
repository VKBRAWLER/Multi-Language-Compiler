#include <stdio.h>
#include <string.h>
#include "ir_generator.h"

int generateIRFromAST(ASTNode *node, IRInstruction ir[]) {
    static int count = 0;
    static int temp_count = 0;

    if (node == NULL) return count;

    char left[10], right[10], result[10];

    // Process left subtree
    if (node->left) {
        if (node->left->left == NULL && node->left->right == NULL) {
            // Leaf node (operand)
            strcpy(left, node->left->value);
        } else {
            // Non-leaf node (subexpression)
            sprintf(left, "t%d", temp_count);
            temp_count++;
            generateIRFromAST(node->left, ir);
        }
    }

    // Process right subtree
    if (node->right) {
        if (node->right->left == NULL && node->right->right == NULL) {
            // Leaf node (operand)
            strcpy(right, node->right->value);
        } else {
            // Non-leaf node (subexpression)
            sprintf(right, "t%d", temp_count);
            temp_count++;
            generateIRFromAST(node->right, ir);
        }
    }

    // Generate IR for the current node
    if (node->left && node->right) {
        sprintf(result, "t%d", temp_count++);
        strcpy(ir[count].opcode, node->value);
        strcpy(ir[count].operand1, left);
        strcpy(ir[count].operand2, right);
        strcpy(ir[count].result, result);
        count++;
    }

    return count;
}

// Recursive Traversal: The function uses recursion to traverse the AST in a post-order manner (process left subtree, right subtree, then the current node).

// Temporary Variables: Temporary variables (t0, t1, etc.) are used to store intermediate results of subexpressions.

// Static Variables: The use of static ensures that count and temp_count retain their values across recursive calls.

// IRInstruction Structure: The code assumes an IRInstruction structure with fields like opcode, operand1, operand2, and result.