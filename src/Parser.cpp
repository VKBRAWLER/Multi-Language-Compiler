#include "Parser.h"
#include "SymbolTable.h"
#include <stdexcept>

std::vector<Node> allFunctions;

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

Token Parser::peek() {
    if (pos < tokens.size())
        return tokens[pos];
    throw std::runtime_error("Unexpected end of input");
}

Token Parser::advance() {
    if (pos < tokens.size())
        return tokens[pos++];
    throw std::runtime_error("Unexpected end of input");
}

bool Parser::match(const std::string& val) {
    if (pos < tokens.size() && tokens[pos].value == val) {
        ++pos;
        return true;
    }
    return false;
}

bool Parser::matchType(const std::string& type) {
    if (pos < tokens.size() && tokens[pos].type == type) {
        ++pos;
        return true;
    }
    return false;
}

Node Parser::parse() {
    Node root = {"Program"};
    while (pos < tokens.size() && tokens[pos].type == "preprocessor") {
        root.children.push_back({"Include: " + tokens[pos].value});
        ++pos;
    }
    while (pos + 2 < tokens.size() &&
           tokens[pos].value == "using" &&
           tokens[pos + 1].value == "namespace" &&
           tokens[pos + 2].type == "identifier") {
        std::string ns = tokens[pos + 2].value;
        root.children.push_back({"Using: namespace " + ns});
        pos += 3;
        if (pos < tokens.size() && tokens[pos].value == ";")
            ++pos;
    }
    while (pos < tokens.size()) {
        Node func = parseFunction();
        root.children.push_back(func);
        allFunctions.push_back(func);
    }
    return root;
}

Node Parser::parseFunction() {
    Node funcNode = {"Function"};
    std::string returnType;
    if (match("int")) returnType = "int";
    else if (match("void")) returnType = "void";
    else if (match("float")) returnType = "float";
    else if (match("double")) returnType = "double";
    else if (match("char")) returnType = "char";
    else if (match("bool")) returnType = "bool";
    else if (match("string")) returnType = "string";
    else throw std::runtime_error("Expected return type");

    Token name = advance();
    if (name.type != "identifier")
        throw std::runtime_error("Expected function name");

    funcNode.children.push_back({"ReturnType: " + returnType});
    funcNode.children.push_back({"FunctionName: " + name.value});

    std::string prevScope = currentScope;
    currentScope = name.value;

    if (!match("("))
        throw std::runtime_error("Expected (");
    Node paramList = {"Parameters"};
    if (!match(")")) {
        do {
            std::string paramType;
            if (match("int")) paramType = "int";
            else if (match("void")) paramType = "void";
            else if (match("float")) paramType = "float";
            else if (match("double")) paramType = "double";
            else if (match("char")) paramType = "char";
            else if (match("bool")) paramType = "bool";
            else if (match("string")) paramType = "string";
            else throw std::runtime_error("Expected parameter type");
            Token paramName = advance();
            if (paramName.type != "identifier")
                throw std::runtime_error("Expected parameter name");
            paramList.children.push_back({paramType + " " + paramName.value});
        } while (match(","));
        if (!match(")"))
            throw std::runtime_error("Expected )");
    }
    funcNode.children.push_back(paramList);

    if (!match("{"))
        throw std::runtime_error("Expected {");

    Node body = {"Body"};
    while (!match("}")) {
        body.children.push_back(parseStatement());
    }
    funcNode.children.push_back(body);

    currentScope = prevScope;
    return funcNode;
}

Node Parser::parseStatement() {
    std::string varType;
    if (match("int")) varType = "int";
    else if (match("void")) varType = "void";
    else if (match("float")) varType = "float";
    else if (match("double")) varType = "double";
    else if (match("char")) varType = "char";
    else if (match("bool")) varType = "bool";
    else if (match("string")) varType = "string";

    if (!varType.empty()) {
        Token varName = advance();
        if (varName.type != "identifier")
            throw std::runtime_error("Expected variable name");
        Node decl = {"VarDecl"};
        decl.children.push_back({varType + " " + varName.value});
        
        // Add to symbol table
        SymbolEntry entry;
        entry.name = varName.value;
        entry.type = varType;
        entry.scope = currentScope;
        entry.hasValue = false;
        symbolTable.push_back(entry);
        
        if (match("=")) {
            Node expr = parseExpression();
            decl.children.push_back(expr);
            // If initializing with a constant, store its value
            if (expr.children[0].label.substr(0, 7) == "Value: ") {
                try {
                    entry.value = std::stoi(expr.children[0].label.substr(7));
                    entry.hasValue = true;
                    symbolTable.back() = entry;  // Update the entry we just added
                } catch (...) {}
            }
        }
        if (!match(";"))
            throw std::runtime_error("Expected ; after variable declaration");
        return decl;
    }

    if (match("return")) {
        Node retNode = {"Return"};
        retNode.children.push_back(parseExpression());
        if (!match(";"))
            throw std::runtime_error("Expected ; after return");
        return retNode;
    }

    if (match("if")) {
        Node ifNode = {"If"};
        if (!match("("))
            throw std::runtime_error("Expected ( after if");
        ifNode.children.push_back(parseExpression());
        if (!match(")"))
            throw std::runtime_error("Expected ) after if condition");
        ifNode.children.push_back(parseStatement());
        if (match("else"))
            ifNode.children.push_back(parseStatement());
        return ifNode;
    }

    if (match("while")) {
        Node whileNode = {"While"};
        if (!match("("))
            throw std::runtime_error("Expected ( after while");
        whileNode.children.push_back(parseExpression());
        if (!match(")"))
            throw std::runtime_error("Expected ) after while condition");
        whileNode.children.push_back(parseStatement());
        return whileNode;
    }

    if (match("for")) {
        Node forNode = {"For"};
        if (!match("("))
            throw std::runtime_error("Expected ( after for");
        if (tokens[pos].value != ";")
            forNode.children.push_back(parseStatement());
        else
            ++pos;
        if (tokens[pos].value != ";")
            forNode.children.push_back(parseExpression());
        else
            forNode.children.push_back({"Expr", {{"Value: 1"}}});
        if (!match(";"))
            throw std::runtime_error("Expected ; after for condition");
        if (tokens[pos].value != ")") {
            size_t savePos = pos;
            Token lookahead = tokens[pos];
            if (lookahead.type == "identifier" && pos + 1 < tokens.size() && tokens[pos + 1].value == "=") {
                Token var = advance();
                match("=");
                Node assign = {"Assignment"};
                assign.children.push_back({"Var: " + var.value});
                assign.children.push_back(parseExpression());
                forNode.children.push_back(assign);
            }
            else {
                forNode.children.push_back(parseExpression());
            }
        }
        else {
            forNode.children.push_back({"Expr", {{"Value: 0"}}});
        }
        if (!match(")"))
            throw std::runtime_error("Expected ) after for header");
        forNode.children.push_back(parseStatement());
        return forNode;
    }

    if (match("cout")) {
        Node coutNode = {"Cout"};
        if (!match("<<"))
            throw std::runtime_error("Expected << after cout");
        coutNode.children.push_back(parseExpression());
        while (match("<<")) {
            coutNode.children.push_back(parseExpression());
        }
        if (!match(";"))
            throw std::runtime_error("Expected ; after cout");
        return coutNode;
    }

    if (match("cin")) {
        Node cinNode = {"Cin"};
        if (!match(">>"))
            throw std::runtime_error("Expected >> after cin");
        do {
            Token var = advance();
            if (var.type != "identifier")
                throw std::runtime_error("Expected variable after >>");
            cinNode.children.push_back({"Var: " + var.value});
        } while (match(">>"));
        if (!match(";"))
            throw std::runtime_error("Expected ; after cin");
        return cinNode;
    }

    if (match("{")) {
        Node block = {"Block"};
        while (!match("}")) {
            block.children.push_back(parseStatement());
        }
        return block;
    }

    Token first = advance();
    if (first.type == "identifier") {
        if (match("=")) {
            Node assign = {"Assignment"};
            assign.children.push_back({"Var: " + first.value});
            Node expr = parseExpression();
            assign.children.push_back(expr);
            
            // Update symbol table on assignment
            if (expr.children[0].label.substr(0, 7) == "Value: ") {
                try {
                    std::string val = expr.children[0].label.substr(7);
                    int value = std::stoi(val);
                    // Find and update the variable in symbol table
                    for (auto& entry : symbolTable) {
                        if (entry.name == first.value && entry.scope == currentScope) {
                            entry.value = value;
                            entry.hasValue = true;
                            break;
                        }
                    }
                } catch (...) {}
            }
            
            if (!match(";"))
                throw std::runtime_error("Expected ; after assignment");
            return assign;
        }
        else if (match("(")) {
            Node call = {"FunctionCall"};
            call.children.push_back({"Callee: " + first.value});
            Node args = {"Arguments"};
            if (!match(")")) {
                do {
                    args.children.push_back(parseExpression());
                } while (match(","));
                if (!match(")"))
                    throw std::runtime_error("Expected ) after function call arguments");
            }
            call.children.push_back(args);
            if (!match(";"))
                throw std::runtime_error("Expected ; after function call");
            return call;
        }
    }
    throw std::runtime_error("Unknown statement starting with: " + first.value);
}

Node Parser::parseExpression() {
    Node left = parseSimpleExpression();
    while (pos < tokens.size() && tokens[pos].type == "symbol" &&
           (tokens[pos].value == "+" || tokens[pos].value == "-" ||
            tokens[pos].value == "*" || tokens[pos].value == "/" ||
            tokens[pos].value == "%" ||
            tokens[pos].value == "==" || tokens[pos].value == "!=" ||
            tokens[pos].value == "<" || tokens[pos].value == ">" ||
            tokens[pos].value == "<=" || tokens[pos].value == ">=")) {
        Token op = advance();
        Node exprNode = {"Expr"};
        exprNode.children.push_back(left);
        exprNode.children.push_back({"Op: " + op.value});
        exprNode.children.push_back(parseSimpleExpression());
        left = exprNode;
    }
    return left;
}

Node Parser::parseSimpleExpression() {
    Token left = advance();
    if (left.type == "identifier" && pos < tokens.size() && tokens[pos].value == "(") {
        advance(); // consume '('
        Node call = {"FunctionCall"};
        call.children.push_back({"Callee: " + left.value});
        Node args = {"Arguments"};
        if (pos < tokens.size() && tokens[pos].value != ")") {
            do {
                args.children.push_back(parseExpression());
            } while (match(","));
        }
        if (!match(")"))
            throw std::runtime_error("Expected ) after function call arguments");
        call.children.push_back(args);
        return call;
    }
    Node exprNode = {"Expr"};
    exprNode.children.push_back({"Value: " + left.value});
    return exprNode;
} 