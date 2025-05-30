#include "Tokenizer.h"
#include <regex>
#include <stdexcept>

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::vector<std::regex> token_patterns = {
        std::regex("#[a-zA-Z_]+[^\\n]*"),
        std::regex("\\bint\\b"),
        std::regex("\\bvoid\\b"),
        std::regex("\\bfloat\\b"),
        std::regex("\\bdouble\\b"),
        std::regex("\\bchar\\b"),
        std::regex("\\bbool\\b"),
        std::regex("\\bstring\\b"),
        std::regex("\\breturn\\b"),
        std::regex("\\bif\\b"),
        std::regex("\\belse\\b"),
        std::regex("\\bwhile\\b"),
        std::regex("\\bfor\\b"),
        std::regex("\\bswitch\\b"),
        std::regex("\\bcase\\b"),
        std::regex("\\bbreak\\b"),
        std::regex("\\bcontinue\\b"),
        std::regex("\\bcout\\b"),
        std::regex("\\bcin\\b"),
        std::regex("\\benum\\b"),
        std::regex("\\bstruct\\b"),
        std::regex("\\bconst\\b"),
        std::regex("\\btrue\\b"),
        std::regex("\\bfalse\\b"),
        std::regex("\"[^\"]*\""),
        std::regex("'[^']*'"),
        std::regex("<<|>>"),
        std::regex("==|!=|<=|>=|<|>"),
        std::regex("[a-zA-Z_][a-zA-Z0-9_]*"),
        std::regex("[0-9]+(\\.[0-9]+)?"),
        std::regex("[(){};,=+*/\\-<>%.\\[\\]:]")
    };

    std::string::const_iterator it = code.begin();
    while (it != code.end()) {
        if (std::isspace(*it)) {
            ++it;
            continue;
        }
        bool matched = false;
        for (const auto& pat : token_patterns) {
            std::smatch match;
            if (std::regex_search(it, code.cend(), match, pat, std::regex_constants::match_continuous)) {
                std::string val = match.str();
                std::string type;
                if (val == "int" || val == "void" || val == "float" || val == "double" ||
                    val == "char" || val == "bool" || val == "string" ||
                    val == "return" || val == "if" || val == "else" || val == "while" ||
                    val == "for" || val == "switch" || val == "case" ||
                    val == "break" || val == "continue" ||
                    val == "cout" || val == "cin" || val == "enum" || val == "struct" ||
                    val == "const" || val == "true" || val == "false")
                    type = "keyword";
                else if (std::regex_match(val, std::regex("#[a-zA-Z_]+[^\\n]*")))
                    type = "preprocessor";
                else if (std::regex_match(val, std::regex("\"[^\"]*\"")))
                    type = "string";
                else if (std::regex_match(val, std::regex("[0-9]+(\\.[0-9]+)?")))
                    type = "number";
                else if (std::regex_match(val, std::regex("[(){};,=+*/\\-<>%.\\[\\]:]")) ||
                         val == "==" || val == "!=" || val == "<=" || val == ">=" || val == "<" || val == ">" ||
                         val == "<<" || val == ">>")
                    type = "symbol";
                else
                    type = "identifier";
                tokens.push_back({type, val});
                it += val.length();
                matched = true;
                break;
            }
        }
        if (!matched)
            throw std::runtime_error("Unrecognized token: " + std::string(it, it + 1));
    }
    return tokens;
} 