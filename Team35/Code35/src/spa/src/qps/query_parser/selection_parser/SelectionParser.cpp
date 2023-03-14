#include <string>
#include <utility>

#include "SelectionParser.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/clause/select_clause/SingleSynonymSelectClause.h"
#include "qps/clause/select_clause/BooleanSelectClause.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_parser/SemanticValidator.h"

SelectionParser::SelectionParser(PQLTokenScanner &pqlTokenScanner,
                                 std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
    pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::unique_ptr<SelectClause> SelectionParser::parse() {
    pqlTokenScanner.matchAndValidate(Token::Tag::Select);

    if (pqlTokenScanner.peek(Token::Tag::Bool)) {  // BOOLEAN
        if (!SemanticValidator::isDeclared(synonyms, "BOOLEAN")) {
            pqlTokenScanner.match(Token::Tag::Bool);
            return std::move(std::make_unique<BooleanSelectClause>());
        } else {
            return std::move(parseSelect());
        }
        pqlTokenScanner.match(Token::Tag::Bool);
        return std::move(std::make_unique<BooleanSelectClause>());
    } else if (pqlTokenScanner.isName()) {  // single synonym
        return std::move(parseSelect());
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<SelectClause> SelectionParser::parseSelect() {
    std::string selected = pqlTokenScanner.peekLexeme();
    Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, selected);
    Synonym selectedSynonym(de, selected);
    std::unique_ptr<SelectClause> selectClause = std::make_unique<SingleSynonymSelectClause>(selectedSynonym);
    pqlTokenScanner.next();
    return std::move(selectClause);
}
