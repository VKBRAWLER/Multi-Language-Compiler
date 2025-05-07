#include <stdio.h>
#include <stdlib.h>

// Function to simulate IR generation
void generateIR(const char *input) {
  printf("Generating IR for input: %s\n", input);
  // Add your IR generation logic here
}

int main() {
  char input[256];
  printf("Enter the input for IR generation: ");
  if (fgets(input, sizeof(input), stdin) != NULL) {
    // Remove newline character if present
    char *newline = strchr(input, '\n');
    if (newline) *newline = '\0';

    generateIR(input);
  } else {
    fprintf(stderr, "Error reading input.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}