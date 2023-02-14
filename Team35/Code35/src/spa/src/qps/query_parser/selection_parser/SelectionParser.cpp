#include <string>
#include "SelectionParser.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "qps/query_exceptions/SyntaxException.h"

Synonym SelectionParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Select);
    std::unique_ptr<Token> selectedSynonym = tokenValidator.validateAndConsumeSynonymToken();
    for (auto synonym : synonyms) {
        if (selectedSynonym->getLexeme() == synonym.str()) {
            return synonym;
        }
    }
    throw SyntaxException();
}
