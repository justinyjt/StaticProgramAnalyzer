#include <string>
#include "SelectionParser.h"

Synonym SelectionParser::parse(std::unique_ptr<Lexer> lexer, std::vector<Synonym> synonyms) {
    Token selectedToken = lexer->Scan();
    for (auto synonym : synonyms) {
        if (synonym.getDeclaration() == selectedToken.GetLexeme()) {
            return synonym;
        }
    }
    throw std::runtime_error("synonym not declared!");
}
