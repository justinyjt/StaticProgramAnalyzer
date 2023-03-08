#include <string>
#include <utility>

#include "SelectionParser.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/clause/SingleSynonymSelectClause.h"
#include "qps/clause/BooleanSelectClause.h"
#include "qps/query_exceptions/SyntaxException.h"

SelectionParser::SelectionParser(PQLTokenScanner &pqlTokenScanner, std::vector<Synonym>& synonyms) :
    pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::unique_ptr<SelectClause> SelectionParser::parse() {
    pqlTokenScanner.matchAndValidate(Token::Tag::Select);

    if (pqlTokenScanner.peek(Token::Tag::Bool) && !isSynonymDeclared("BOOLEAN")) {  // BOOLEAN
        pqlTokenScanner.match(Token::Tag::Bool);
        return std::move(std::make_unique<BooleanSelectClause>());
    } else if (isName(pqlTokenScanner.peekLexeme())) {  // single synonym
        return std::move(parseSelect());
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<SelectClause> SelectionParser::parseSelect() {
    for (auto synonym : synonyms) {
        if (pqlTokenScanner.peekLexeme() == synonym.str()) {
            std::unique_ptr<SelectClause> selectClause = std::make_unique<SingleSynonymSelectClause>(synonym);
            pqlTokenScanner.next();
            return std::move(selectClause);
        }
    }
    throw SemanticException();
}

bool SelectionParser::isSynonymDeclared(std::string selectedSynonym) {
    for (auto synonym : synonyms) {
        if (selectedSynonym == synonym.str()) {
            return true;
        }
    }
    return false;
}

bool SelectionParser::isName(std::string input) {
    if (!isalpha(input[0])) {
        return false;
    }
    for (char c : input) {
        if (!isalpha(c) && !isdigit(c)) {
            return false;
        }
    }
    return true;
}
