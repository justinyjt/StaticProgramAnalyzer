#include <string>
#include "SelectionParser.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

Synonym SelectionParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    std::unique_ptr<Token> selectedToken = tokenValidator.getNextToken();;
    for (auto synonym : synonyms) {
        if (synonym.getValue() == selectedToken->getLexeme()) {
            return synonym;
        }
    }
    throw std::runtime_error("synonym not declared!");
}
