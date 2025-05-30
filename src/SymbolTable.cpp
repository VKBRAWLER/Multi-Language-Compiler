#include "SymbolTable.h"
#include <unordered_map>

std::vector<SymbolEntry> symbolTable;

int evalExpr(const Node& expr, std::unordered_map<std::string, int>& vars) {
    if (expr.label == "Expr") {
        if (expr.children.size() == 1) {
            std::string val = expr.children[0].label.substr(7); // "Value: "
            if (!val.empty() && std::isdigit(val[0]))
                return std::stoi(val);
            if (vars.count(val))
                return vars[val];
            return 0;
        }
        else if (expr.children.size() == 3) {
            int left = evalExpr(expr.children[0], vars);
            std::string op = expr.children[1].label.substr(4); // "Op: "
            int right = evalExpr(expr.children[2], vars);
            if (op == "+") return left + right;
            if (op == "-") return left - right;
            if (op == "*") return left * right;
            if (op == "/") return right != 0 ? left / right : 0;
            if (op == "%") return right != 0 ? left % right : 0;
            if (op == "==") return left == right;
            if (op == "!=") return left != right;
            if (op == "<") return left < right;
            if (op == ">") return left > right;
            if (op == "<=") return left <= right;
            if (op == ">=") return left >= right;
        }
    }
    return 0;
} 