#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include "Node.h"

struct SymbolEntry {
    std::string name;
    std::string type;
    std::string scope;
    int value;
    bool hasValue;
};

extern std::vector<SymbolEntry> symbolTable;
int evalExpr(const Node& expr, std::unordered_map<std::string, int>& vars);

#endif // SYMBOL_TABLE_H 