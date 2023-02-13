#include <string>
#include "SelectionParser.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_exceptions/SemanticException.h"

Synonym SelectionParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Select);
    std::unique_ptr<Token> selectedSynonym = tokenValidator.validateAndConsumeSynonymToken();
    for (auto synonym : synonyms) {
        if (selectedSynonym->getLexeme() == synonym.getValue()) {
            return synonym;
        }
    }
    throw SemanticException();
}
