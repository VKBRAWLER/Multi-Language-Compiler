# Parse Tree Generator

A C++ program that parses C++ code and generates a parse tree visualization along with execution traces and symbol table information.

## Project Structure

```
.
├── include/
│   ├── Token.h
│   ├── Node.h
│   ├── Parser.h
│   ├── Tokenizer.h
│   ├── SymbolTable.h
│   ├── TraceGenerator.h
│   └── json.hpp
├── src/
│   ├── Node.cpp
│   ├── Parser.cpp
│   ├── Tokenizer.cpp
│   ├── SymbolTable.cpp
│   └── TraceGenerator.cpp
├── CMakeLists.txt
└── README.md
```

## Run the program
```bash
g++ main.cpp src/*.cpp -I./include -I. -o main ; if ($?) { ./main }
```

## Usage

1. Create an input C++ file named `input.cpp` in the same directory as the executable
2. Run the parser:
   ```bash
   ./parser
   ```
3. The program will generate three output files:
   - `tree.json`: Contains the parse tree visualization
   - `trace.json`: Contains the execution trace
   - `symbol_table.json`: Contains the symbol table information

## Features

- Tokenizes C++ code
- Generates abstract syntax tree
- Supports basic C++ constructs:
  - Variable declarations
  - Function definitions
  - Control structures (if, while, for)
  - Input/Output operations (cin, cout)
  - Basic expressions and operators
- Generates execution traces
- Maintains symbol table
- Outputs results in JSON format for easy visualization