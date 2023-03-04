#include <string>
#include "SelectionParser.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/clause/SingleSynonymSelectClause.h"
#include "qps/clause/BooleanSelectClause.h"

// Return a clause instead
std::unique_ptr<SelectClause> SelectionParser::parse(TokenValidator& tokenValidator, std::vector<Synonym>& synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Select);

    if (tokenValidator.isNextTokenType(Token::Tag::Bool) && !isSynonymDeclared("BOOLEAN", synonyms)) { // BOOLEAN
        tokenValidator.validateAndConsumeTokenType(Token::Tag::Bool);
        return std::move(std::make_unique<BooleanSelectClause>());
    } else if (tokenValidator.isNextTokenValidName()) { // single synonym
        std::unique_ptr<Token> selectedSynonym = tokenValidator.validateAndConsumeSynonymToken();
        for (auto synonym : synonyms) {
            if (selectedSynonym->getLexeme() == synonym.str()) {
                std::unique_ptr<SelectClause> selectClause = std::make_unique<SingleSynonymSelectClause>(synonym);
                return std::move(selectClause);
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
