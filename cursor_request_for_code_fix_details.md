
**User**

what are the steps of this project and what is it's purpose

---

**Cursor**

This project is a C++ Code Parser and Visualizer. Its main purpose is to analyze, visualize, and simulate the execution of C++ code. Here's a detailed breakdown:

**Purpose:**
1. Parse C++ code and create a visual representation of its structure
2. Track variables and their values during execution
3. Generate a trace of how the code executes
4. Help understand program flow and structure

**Main Components and Steps:**

1. **Tokenization** (`Tokenizer.h/cpp`)
   - Takes raw C++ source code as input
   - Breaks it into individual tokens (keywords, identifiers, operators, etc.)
   - Example: `int x = 5;` becomes tokens: `["int", "x", "=", "5", ";"]`

2. **Parsing** (`Parser.h/cpp`)
   - Takes tokens and builds an Abstract Syntax Tree (AST)
   - Handles different C++ constructs:
     - Function declarations
     - Variable declarations
     - Control structures (if, while, for)
     - Expressions and operators
     - Input/Output operations (cin, cout)

3. **Symbol Table Management** (`SymbolTable.h/cpp`)
   - Tracks all variables in the program
   - Records for each variable:
     - Name
     - Type (int, double, etc.)
     - Scope (which function it belongs to)
     - Current value (if known)

4. **Execution Simulation** (`TraceGenerator.h/cpp`)
   - Simulates how the code would execute
   - Tracks variable changes
   - Records program flow
   - Generates an execution trace

5. **Output Generation** (in `main.cpp`)
   Creates three JSON files:
   ```cpp
   a) tree.json       // Shows program structure
   b) trace.json      // Shows execution flow
   c) symbol_table.json // Shows variable information
   ```

**Example Flow:**
```cpp
Input:
int main() {
    int x = 5;
    int y = x * 2;
    return 0;
}

Output Files:
1. tree.json (Structure):
   - Program
     - Function: main
       - Variables: x, y
       - Operations: assignment, multiplication
       - Return statement

2. symbol_table.json (Variables):
   - x: int, main scope, value 5
   - y: int, main scope, value 10

3. trace.json (Execution):
   - Declared x
   - Assigned 5 to x
   - Declared y
   - Computed x * 2
   - Assigned 10 to y
```

**Project Features:**
1. **Code Analysis**
   - Syntax validation
   - Structure analysis
   - Variable tracking

2. **Visualization Support**
   - JSON output for easy visualization
   - Tree structure representation
   - Execution flow tracking

3. **Error Handling**
   - Syntax error detection
   - Runtime error simulation
   - Detailed error messages

4. **Language Support**
   - Basic C++ constructs
   - Function definitions
   - Control structures
   - Input/Output operations
   - Variable declarations and assignments

This project would be particularly useful for:
1. Learning C++ programming
2. Debugging complex code
3. Understanding program flow
4. Teaching programming concepts
5. Code analysis and visualization tools

The project follows a compiler-like architecture but focuses on visualization and analysis rather than generating machine code.

```cpp
   a) tree.json       // Shows program structure
   b) trace.json      // Shows execution flow
   c) symbol_table.json // Shows variable information
```

```cpp
Input:
int main() {
    int x = 5;
    int y = x * 2;
    return 0;
}

Output Files:
1. tree.json (Structure):
   - Program
     - Function: main
       - Variables: x, y
       - Operations: assignment, multiplication
       - Return statement

2. symbol_table.json (Variables):
   - x: int, main scope, value 5
   - y: int, main scope, value 10

3. trace.json (Execution):
   - Declared x
   - Assigned 5 to x
   - Declared y
   - Computed x * 2
   - Assigned 10 to y
```

---

**User**

what kind of parsing method is used

---

**Cursor**

This project uses Recursive Descent Parsing, which is a top-down parsing technique. Let me break down how it works in this code:

1. **Top-Level Parsing** (`Parser::parse()`):
```cpp
Node Parser::parse() {
    Node root = {"Program"};
    // Parse includes (#include statements)
    while (pos < tokens.size() && tokens[pos].type == "preprocessor") {
        root.children.push_back({"Include: " + tokens[pos].value});
    }
    // Parse using namespace
    while (pos + 2 < tokens.size() && tokens[pos].value == "using") {
        // ...
    }
    // Parse functions
    while (pos < tokens.size()) {
        Node func = parseFunction();
        root.children.push_back(func);
    }
}
```

2. **Recursive Functions for Different Constructs:**

   a) **Function Parsing** (`parseFunction()`):
   ```cpp
   Node Parser::parseFunction() {
       // Parse return type
       if (match("int")) returnType = "int";
       // Parse function name
       Token name = advance();
       // Parse parameters
       if (!match("("))
           throw std::runtime_error("Expected (");
       // Parse body
       Node body = {"Body"};
       while (!match("}")) {
           body.children.push_back(parseStatement());
       }
   }
   ```

   b) **Statement Parsing** (`parseStatement()`):
   ```cpp
   Node Parser::parseStatement() {
       // Variable declarations
       if (match("int") || match("double") /*...*/) {
           // Parse variable declaration
       }
       // If statements
       if (match("if")) {
           // Parse condition and body
       }
       // While loops
       if (match("while")) {
           // Parse condition and body
       }
       // For loops
       if (match("for")) {
           // Parse initialization, condition, increment
       }
   }
   ```

   c) **Expression Parsing** (`parseExpression()`):
   ```cpp
   Node Parser::parseExpression() {
       Node left = parseSimpleExpression();
       // Handle operators (+, -, *, /, etc.)
       while (isOperator(peek())) {
           Token op = advance();
           Node right = parseSimpleExpression();
       }
   }
   ```

Key characteristics of this Recursive Descent Parser:

1. **Helper Methods**:
   ```cpp
   Token peek()     // Look ahead without consuming
   Token advance()  // Consume and return next token
   bool match(str)  // Check if current token matches
   ```

2. **Error Handling**:
   ```cpp
   if (!match(";"))
       throw std::runtime_error("Expected ; after statement");
   ```

3. **Grammar Rules** are implemented as functions:
   - `parseFunction()` for function declarations
   - `parseStatement()` for statements
   - `parseExpression()` for expressions
   - `parseSimpleExpression()` for basic expressions

4. **Predictive Parsing**:
   - Looks at the next token to decide what to parse
   - No backtracking needed
   - Each function knows what tokens it expects

Example of how it parses:
```cpp
int main() {
    int x = 5;
}

Parsing Steps:
1. parse() sees "int" -> calls parseFunction()
2. parseFunction() processes:
   - Return type "int"
   - Name "main"
   - Parameters "()"
   - Body "{...}"
3. parseStatement() inside body sees:
   - Type "int" -> variable declaration
   - Name "x"
   - Assignment "="
   - Value "5"
```

This parsing method is:
1. **Intuitive**: Each grammar rule maps to a function
2. **Efficient**: O(n) time complexity for most inputs
3. **Flexible**: Easy to add new language features
4. **Maintainable**: Clear separation of concerns
5. **Error-Friendly**: Good error reporting capabilities

The parser builds an Abstract Syntax Tree (AST) that represents the program's structure, which is then used for:
- Code analysis
- Symbol table generation
- Execution simulation
- Visualization

```cpp
Node Parser::parse() {
    Node root = {"Program"};
    // Parse includes (#include statements)
    while (pos < tokens.size() && tokens[pos].type == "preprocessor") {
        root.children.push_back({"Include: " + tokens[pos].value});
    }
    // Parse using namespace
    while (pos + 2 < tokens.size() && tokens[pos].value == "using") {
        // ...
    }
    // Parse functions
    while (pos < tokens.size()) {
        Node func = parseFunction();
        root.children.push_back(func);
    }
}
```

```cpp
   Node Parser::parseFunction() {
       // Parse return type
       if (match("int")) returnType = "int";
       // Parse function name
       Token name = advance();
       // Parse parameters
       if (!match("("))
           throw std::runtime_error("Expected (");
       // Parse body
       Node body = {"Body"};
       while (!match("}")) {
           body.children.push_back(parseStatement());
       }
   }
```

```cpp
   Node Parser::parseStatement() {
       // Variable declarations
       if (match("int") || match("double") /*...*/) {
           // Parse variable declaration
       }
       // If statements
       if (match("if")) {
           // Parse condition and body
       }
       // While loops
       if (match("while")) {
           // Parse condition and body
       }
       // For loops
       if (match("for")) {
           // Parse initialization, condition, increment
       }
   }
```

```cpp
   Node Parser::parseExpression() {
       Node left = parseSimpleExpression();
       // Handle operators (+, -, *, /, etc.)
       while (isOperator(peek())) {
           Token op = advance();
           Node right = parseSimpleExpression();
       }
   }
```

```cpp
   Token peek()     // Look ahead without consuming
   Token advance()  // Consume and return next token
   bool match(str)  // Check if current token matches
```

```cpp
   if (!match(";"))
       throw std::runtime_error("Expected ; after statement");
```

```cpp
int main() {
    int x = 5;
}

Parsing Steps:
1. parse() sees "int" -> calls parseFunction()
2. parseFunction() processes:
   - Return type "int"
   - Name "main"
   - Parameters "()"
   - Body "{...}"
3. parseStatement() inside body sees:
   - Type "int" -> variable declaration
   - Name "x"
   - Assignment "="
   - Value "5"
```

