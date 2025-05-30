#include "TraceGenerator.h"
#include "Parser.h"
#include "SymbolTable.h"

std::vector<json> trace;

void simulateExecution(const Node& node, std::unordered_map<std::string, int>& vars) {
    if (node.label == "Function") {
        std::string funcName;
        for (const auto& child : node.children) {
            if (child.label.rfind("FunctionName:", 0) == 0) {
                funcName = child.label.substr(13);
                break;
            }
        }
        if (!funcName.empty()) {
            trace.push_back({{"action", "call"}, {"function", funcName}});
            for (const auto& child : node.children) {
                if (child.label == "Body") {
                    for (const auto& stmt : child.children) {
                        simulateExecution(stmt, vars);
                    }
                }
            }
            trace.push_back({{"action", "return"}, {"function", funcName}});
        }
    }
    else if (node.label == "VarDecl") {
        std::string var;
        if (!node.children.empty()) {
            std::string decl = node.children[0].label;
            size_t space = decl.find(' ');
            var = (space != std::string::npos) ? decl.substr(space + 1) : decl;
        }
        int val = 0;
        if (node.children.size() > 1)
            val = evalExpr(node.children[1], vars);
        vars[var] = val;
        trace.push_back({{"action", "vardecl"}, {"variable", var}});
    }
    else if (node.label == "Assignment") {
        std::string var;
        if (!node.children.empty())
            var = node.children[0].label.substr(5); // "Var: "
        int val = 0;
        if (node.children.size() > 1)
            val = evalExpr(node.children[1], vars);
        vars[var] = val;
        trace.push_back({{"action", "assign"}, {"variable", var}});
    }
    else if (node.label == "Return") {
        trace.push_back({{"action", "return_stmt"}});
        if (!node.children.empty())
            evalExpr(node.children[0], vars);
    }
    else if (node.label == "If") {
        trace.push_back({{"action", "if_enter"}});
        bool conditionTrue = false;
        if (!node.children.empty())
            conditionTrue = evalExpr(node.children[0], vars);
        if (conditionTrue) {
            trace.push_back({{"action", "if_taken"}, {"branch", "then"}});
            if (node.children.size() > 1)
                simulateExecution(node.children[1], vars);
        }
        else {
            trace.push_back({{"action", "if_taken"}, {"branch", "else"}});
            if (node.children.size() > 2)
                simulateExecution(node.children[2], vars);
        }
    }
    else if (node.label == "While") {
        trace.push_back({{"action", "while_enter"}});
        int loopCount = 0;
        while (evalExpr(node.children[0], vars) && loopCount < 10) {
            if (node.children.size() > 1)
                simulateExecution(node.children[1], vars);
            loopCount++;
        }
    }
    else if (node.label == "For") {
        trace.push_back({{"action", "for_enter"}});
        std::unordered_map<std::string, int> localVars = vars;
        if (!node.children.empty())
            simulateExecution(node.children[0], localVars);
        int loopCount = 0;
        while ((node.children.size() > 1 ? evalExpr(node.children[1], localVars) : true) && loopCount < 10) {
            if (node.children.size() > 3)
                simulateExecution(node.children[3], localVars);
            if (node.children.size() > 2)
                simulateExecution(node.children[2], localVars);
            loopCount++;
        }
        for (const auto& kv : localVars)
            vars[kv.first] = kv.second;
    }
    else if (node.label == "Cout") {
        trace.push_back({{"action", "cout"}});
        for (const auto& child : node.children)
            evalExpr(child, vars);
    }
    else if (node.label == "Cin") {
        trace.push_back({{"action", "cin"}});
        for (const auto& child : node.children) {
            if (child.label.rfind("Var: ", 0) == 0) {
                std::string var = child.label.substr(5);
                if (vars.count(var) == 0)
                    vars[var] = 5;
            }
        }
    }
    else if (node.label == "FunctionCall") {
        std::string callee;
        for (const auto& child : node.children) {
            if (child.label.rfind("Callee:", 0) == 0) {
                callee = child.label.substr(7);
                break;
            }
        }
        if (!callee.empty()) {
            trace.push_back({{"action", "call"}, {"function", callee}});
            for (const auto& func : allFunctions) {
                std::string fname;
                for (const auto& fchild : func.children) {
                    if (fchild.label.rfind("FunctionName:", 0) == 0) {
                        fname = fchild.label.substr(13);
                        break;
                    }
                }
                if (fname == callee) {
                    simulateExecution(func, vars);
                    break;
                }
            }
            trace.push_back({{"action", "return"}, {"function", callee}});
        }
    }
    else {
        for (const auto& child : node.children) {
            simulateExecution(child, vars);
        }
    }
} 