#include <string>
#include "SelectionParser.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "qps/query_exceptions/SemanticException.h"

// Return a clause instead
Synonym SelectionParser::parse(TokenValidator& tokenValidator, std::vector<Synonym>& synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Select);

    if (tokenValidator.isNextTokenType(Token::Tag::Bool) && !isSynonymDeclared("BOOLEAN", synonyms)) { // single synonym
        tokenValidator.validateAndConsumeTokenType(Token::Tag::Bool);
        // return clause
        return synonyms.front();
    } else if (tokenValidator.isNextTokenValidName()) { // BOOLEAN
        std::unique_ptr<Token> selectedSynonym = tokenValidator.validateAndConsumeSynonymToken();
        for (auto synonym : synonyms) {
            if (selectedSynonym->getLexeme() == synonym.str()) {
                return synonym;
            }
        }
    } else {
        throw SemanticException();
    }
}

bool SelectionParser::isSynonymDeclared(std::string selectedSynonym, std::vector<Synonym>& synonyms) {
    for (auto synonym : synonyms) {
        if (selectedSynonym == synonym.str()) {
            return true;
        }
    }
    return false;
}
