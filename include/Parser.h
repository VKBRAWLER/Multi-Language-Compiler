#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "Token.h"
#include "Node.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos = 0;
    std::string currentScope = "global";

    Token peek();
    Token advance();
    bool match(const std::string& val);
    bool matchType(const std::string& type);
    Node parseFunction();
    Node parseStatement();
    Node parseExpression();
    Node parseSimpleExpression();

public:
    Parser(const std::vector<Token>& tokens);
    Node parse();
};

extern std::vector<Node> allFunctions;

#endif // PARSER_H 