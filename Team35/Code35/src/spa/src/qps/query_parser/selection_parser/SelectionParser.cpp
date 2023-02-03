#include <string>
#include "SelectionParser.h"

std::string SelectionParser::parse(Lexer &lexer, std::vector<std::string> synonyms) {
    Token selectedToken = lexer.Scan();
    if (std::count(synonyms.begin(), synonyms.end(), selectedToken.GetLexeme())) {
        return selectedToken.GetLexeme();
    } else {
        throw std::runtime_error("synonym not declared!");
    }
}