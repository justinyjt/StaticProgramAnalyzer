#include <string>
#include "SelectionParser.h"

Synonym SelectionParser::parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms) {
    Token selectedToken = lexer->scan();
    for (auto synonym : synonyms) {
        if (synonym.getValue() == selectedToken.getLexeme()) {
            return synonym;
        }
    }
    throw std::runtime_error("synonym not declared!");
}
