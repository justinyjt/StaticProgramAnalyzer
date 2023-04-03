#include <string>
#include <utility>
#include <vector>

#include "SelectionParser.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/clause/select_clause/BooleanSelectClause.h"
#include "qps/query_parser/SemanticValidator.h"
#include "qps/query_parser/helper.h"
#include "qps/pql/AttrRef.h"

SelectionParser::SelectionParser(PQLTokenScanner &pqlTokenScanner,
                                 std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
    pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::unique_ptr<SelectClause> SelectionParser::parse() {
    pqlTokenScanner.match(Token::Tag::Select);

    if (pqlTokenScanner.peek(Token::Tag::Bool)) {  // BOOLEAN
        if (!SemanticValidator::isDeclared(synonyms, "BOOLEAN")) {
            pqlTokenScanner.match(Token::Tag::Bool);
            return std::move(std::make_unique<BooleanSelectClause>());
        } else {
            std::vector<std::unique_ptr<Synonym>> selectedSynonyms;
            selectedSynonyms.emplace_back(std::move(parseSelect()));
            std::unique_ptr<SelectClause> selectClause = std::make_unique<MultipleSynonymSelectClause>(
                std::move(selectedSynonyms));
            return selectClause;
        }
    } else if (pqlTokenScanner.peek(Token::Tag::LessThan)) {
        pqlTokenScanner.match(Token::Tag::LessThan);
        std::unique_ptr<SelectClause> selectClause = std::make_unique<MultipleSynonymSelectClause>(
            std::move(parseMultiSelect()));
        return selectClause;
    } else {  // single synonym
        std::vector<std::unique_ptr<Synonym>> selectedSynonyms;
        selectedSynonyms.emplace_back(std::move(parseSelect()));
        std::unique_ptr<SelectClause> selectClause = std::make_unique<MultipleSynonymSelectClause>(
            std::move(selectedSynonyms));
        return selectClause;
    }
}

std::vector<std::unique_ptr<Synonym>> SelectionParser::parseMultiSelect() {
    // while loop to get all synonyms and create a multi select clause
    std::vector<std::unique_ptr<Synonym>> selectedSynonymLst;
    while (true) {
        selectedSynonymLst.push_back(std::move(parseSelect()));
        if (pqlTokenScanner.peek(Token::Tag::Comma)) {
            pqlTokenScanner.match(Token::Tag::Comma);
            continue;
        } else {  // ">", end of tuple
            pqlTokenScanner.match(Token::Tag::GreaterThan);
            break;
        }
    }
    return std::move(selectedSynonymLst);
}

std::unique_ptr<Synonym> SelectionParser::parseSelect() {
    // need to add checking for attrRef
    std::string selected = pqlTokenScanner.peekLexeme();
    Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, selected);
    pqlTokenScanner.next();
    if (!pqlTokenScanner.peek(Token::Tag::Dot)) {
        std::unique_ptr<Synonym> selectedSynonym = std::make_unique<Synonym>(de, selected);
        return std::move(selectedSynonym);
    } else {
        pqlTokenScanner.match(Token::Tag::Dot);
        // parse attrName and create AttrRef Object
        return parseAttrRef(de, selected);
    }
}

std::unique_ptr<Synonym> SelectionParser::parseAttrRef(Synonym::DesignEntity de, std::string &selected) {
    std::string attrName = pqlTokenScanner.peekLexeme();
    if (attrName == PROCNAME_KEYWORD) {
        if (de == Synonym::DesignEntity::PROCEDURE || de == Synonym::DesignEntity::CALL) {
            pqlTokenScanner.next();
            std::unique_ptr<AttrRef> selectedSynonym = std::make_unique<AttrRef>(de, selected, PROCNAME_KEYWORD);
            return std::move(selectedSynonym);
        }
        throw SemanticException();
    } else if (attrName == VARNAME_KEYWORD) {
        if (de == Synonym::DesignEntity::VARIABLE || de == Synonym::DesignEntity::READ ||
            de == Synonym::DesignEntity::PRINT) {
            pqlTokenScanner.next();
            std::unique_ptr<AttrRef> selectedSynonym = std::make_unique<AttrRef>(de, selected, VARNAME_KEYWORD);
            return std::move(selectedSynonym);
        }
        throw SemanticException();
    } else if (attrName == VALUE_KEYWORD) {
        if (de == Synonym::DesignEntity::CONSTANT) {
            pqlTokenScanner.next();
            std::unique_ptr<AttrRef> selectedSynonym = std::make_unique<AttrRef>(de, selected, VALUE_KEYWORD);
            return std::move(selectedSynonym);
        }
        throw SemanticException();
    } else {  // STMT_KEYWORD
        pqlTokenScanner.next();
        if (de == Synonym::DesignEntity::STMT || de == Synonym::DesignEntity::READ ||
            de == Synonym::DesignEntity::PRINT || de == Synonym::DesignEntity::CALL ||
            de == Synonym::DesignEntity::WHILE || de == Synonym::DesignEntity::IF ||
            de == Synonym::DesignEntity::ASSIGN) {
            pqlTokenScanner.next();
            std::unique_ptr<AttrRef> selectedSynonym = std::make_unique<AttrRef>(de, selected, STMT_KEYWORD);
            return std::move(selectedSynonym);
        }
        throw SemanticException();
    }
}
