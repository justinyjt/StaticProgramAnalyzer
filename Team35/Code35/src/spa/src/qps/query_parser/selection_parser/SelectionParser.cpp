#include <string>
#include <utility>
#include <vector>

#include "SelectionParser.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/clause/select_clause/SingleSynonymSelectClause.h"
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
            return std::move(parseSelect());
        }
    } else if (pqlTokenScanner.peek(Token::Tag::LessThan)) {
        pqlTokenScanner.match(Token::Tag::LessThan);
        return std::move(parseMultiSelect());
    } else if (pqlTokenScanner.isName()) {  // single synonym
        return std::move(parseSelect());
    }
}

std::unique_ptr<MultipleSynonymSelectClause> SelectionParser::parseMultiSelect() {
    // while loop to get all synonyms and create a multi select clause
    std::vector<std::unique_ptr<SingleSynonymSelectClause>> multiSelectClause;
    while (true) {
        multiSelectClause.push_back(std::move(parseSelect()));
        if (pqlTokenScanner.peek(Token::Tag::Comma)) {
            pqlTokenScanner.match(Token::Tag::Comma);
            continue;
        } else if (pqlTokenScanner.peek(Token::Tag::SemiColon)) {
            pqlTokenScanner.match(Token::Tag::SemiColon);
            break;
        }
    }
    std::unique_ptr<MultipleSynonymSelectClause> selectClause =
            std::make_unique<MultipleSynonymSelectClause>(std::move(multiSelectClause));
    return std::move(selectClause);
}

std::unique_ptr<SingleSynonymSelectClause> SelectionParser::parseSelect() {
    // need to add checking for attrRef
    std::string selected = pqlTokenScanner.peekLexeme();
    Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, selected);
    pqlTokenScanner.next();
    if (!pqlTokenScanner.peek(Token::Tag::Dot)) {
        std::unique_ptr<Synonym> selectedSynonym = std::make_unique<Synonym>(de, selected);
        std::unique_ptr<SingleSynonymSelectClause> selectClause =
                std::make_unique<SingleSynonymSelectClause>(std::move(selectedSynonym));
        return std::move(selectClause);
    } else {
        pqlTokenScanner.match(Token::Tag::Dot);
        // parse attrName and create AttrRef Object
        std::string attrName = pqlTokenScanner.peekLexeme();
        if (attrName == PROCNAME_KEYWORD) {
            if (de == Synonym::DesignEntity::PROCEDURE || de == Synonym::DesignEntity::CALL) {
                pqlTokenScanner.next();
                std::unique_ptr<AttrRef> selectedSynonym = std::make_unique<AttrRef>(de, selected, PROCNAME_KEYWORD);
                std::unique_ptr<SingleSynonymSelectClause> selectClause =
                        std::make_unique<SingleSynonymSelectClause>(std::move(selectedSynonym));
                return std::move(selectClause);
            }
            throw SemanticException();
        } else if (attrName == VARNAME_KEYWORD) {
            if (de == Synonym::DesignEntity::VARIABLE || de == Synonym::DesignEntity::READ ||
                de == Synonym::DesignEntity::PRINT) {
                pqlTokenScanner.next();
                std::unique_ptr<AttrRef> selectedSynonym = std::make_unique<AttrRef>(de, selected, VARNAME_KEYWORD);
                std::unique_ptr<SingleSynonymSelectClause> selectClause =
                        std::make_unique<SingleSynonymSelectClause>(std::move(selectedSynonym));
                return std::move(selectClause);
            }
            throw SemanticException();
        } else if (attrName == VALUE_KEYWORD) {
            if (de == Synonym::DesignEntity::CONSTANT) {
                pqlTokenScanner.next();
                std::unique_ptr<AttrRef> selectedSynonym = std::make_unique<AttrRef>(de, selected, VALUE_KEYWORD);
                std::unique_ptr<SingleSynonymSelectClause> selectClause =
                        std::make_unique<SingleSynonymSelectClause>(std::move(selectedSynonym));
                return std::move(selectClause);
            }
            throw SemanticException();
        } else if (attrName == STMT_KEYWORD) {
            pqlTokenScanner.next();
            if (de == Synonym::DesignEntity::STMT || de == Synonym::DesignEntity::READ ||
                de == Synonym::DesignEntity::PRINT || de == Synonym::DesignEntity::CALL ||
                de == Synonym::DesignEntity::WHILE || de == Synonym::DesignEntity::IF ||
                de == Synonym::DesignEntity::ASSIGN) {
                pqlTokenScanner.next();
                std::unique_ptr<AttrRef> selectedSynonym = std::make_unique<AttrRef>(de, selected, STMT_KEYWORD);
                std::unique_ptr<SingleSynonymSelectClause> selectClause =
                        std::make_unique<SingleSynonymSelectClause>(std::move(selectedSynonym));
                return std::move(selectClause);
            }
            throw SemanticException();
        }
    }
}
