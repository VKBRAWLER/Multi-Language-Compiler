#include <iostream>
#include <fstream>
#include <sstream>
#include "Token.h"
#include "Node.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "SymbolTable.h"
#include "TraceGenerator.h"
using namespace std;

int main() {
    ifstream file("input.cpp");
    if (!file.is_open()) {
        cerr << "Failed to open input.cpp\n";
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();

    try {
        cout << "Starting tokenization...\n";
        auto tokens = tokenize(code);
        cout << "Tokenization complete.\n\n";

        cout << "Starting parsing...\n";
        Parser parser(tokens);
        Node tree = parser.parse();
        json output = nodeToJson(tree);
        cout << "Parsing complete.\n\n";

        cout << "Starting execution simulation...\n";
        // Simulate execution starting from main
        for (const auto& func : allFunctions) {
            for (const auto& fchild : func.children) {
                if (fchild.label == "FunctionName: main") {
                    unordered_map<string, int> vars;
                    simulateExecution(func, vars);
                }
            }
        }
        cout << "Execution simulation complete.\n\n";

        cout << "Writing parse tree to tree.json...\n";
        // Write parse tree
        ofstream out("tree.json");
        out << output.dump(4);
        cout << "Parse tree written successfully.\n\n";

        cout << "Writing execution trace to trace.json...\n";
        // Write trace
        ofstream traceOut("trace.json");
        traceOut << json(trace).dump(4);
        cout << "Execution trace written successfully.\n\n";

        cout << "Generating symbol table...\n";
        // Write symbol table
        json symtab = json::array();
        for (const auto& entry : symbolTable) {
            json row;
            row["name"] = entry.name;
            row["type"] = entry.type;
            row["scope"] = entry.scope;
            if (entry.hasValue)
                row["value"] = entry.value;
            symtab.push_back(row);
        }
        ofstream symtabOut("symbol_table.json");
        symtabOut << symtab.dump(4);
        cout << "Symbol table written successfully.\n\n";

        cout << "All tasks completed successfully!\n";
        cout << "\nOutput files generated:\n";
        cout << "- tree.json (Parse tree)\n";
        cout << "- trace.json (Execution trace)\n";
        cout << "- symbol_table.json (Symbol table)\n";
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}