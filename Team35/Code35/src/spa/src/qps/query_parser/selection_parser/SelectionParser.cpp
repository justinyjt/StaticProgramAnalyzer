#include <string>
#include "SelectionParser.h"

Synonym SelectionParser::parse(const std::unique_ptr<ILexer> &lexer, std::vector<Synonym> synonyms) {
    std::unique_ptr<Token> selectedToken = lexer->scan();
    for (auto synonym : synonyms) {
        if (synonym.getValue() == selectedToken->getLexeme()) {
            return synonym;
        }
    }
    throw std::runtime_error("synonym not declared!");
}
