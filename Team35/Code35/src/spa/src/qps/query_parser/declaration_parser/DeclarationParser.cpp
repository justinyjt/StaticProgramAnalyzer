#include <cassert>

#include "DeclarationParser.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/query_parser/SemanticValidator.h"

DeclarationParser::DeclarationParser(PQLTokenScanner &pqlTokenScanner,
                                     std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
        pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::unordered_map<std::string, Synonym::DesignEntity> DeclarationParser::parse() {
    // check for next declaration
    while (true) {
        if (pqlTokenScanner.peek(Token::Tag::Select) || pqlTokenScanner.peek(Token::Tag::EndOfFile)) {
            break;
        }

        Synonym::DesignEntity de = parseDesignEntity();

        // check if there are multiple declarations of the same type
        parseDeclarations(de);
    }
    return synonyms;
}

// convert Token tag to Synonym design entity
Synonym::DesignEntity DeclarationParser::parseDesignEntity() {
    if (pqlTokenScanner.peekDesignEntity()) {
        Synonym::DesignEntity de;
        switch (pqlTokenScanner.peekTag()) {
            case Token::Tag::Statement:
                de = Synonym::DesignEntity::STMT;
                break;
            case Token::Tag::Read:
                de = Synonym::DesignEntity::READ;
                break;
            case Token::Tag::Print:
                de = Synonym::DesignEntity::PRINT;
                break;
            case Token::Tag::Call:
                de = Synonym::DesignEntity::CALL;
                break;
            case Token::Tag::While:
                de = Synonym::DesignEntity::WHILE;
                break;
            case Token::Tag::If:
                de = Synonym::DesignEntity::IF;
                break;
            case Token::Tag::Assign:
                de = Synonym::DesignEntity::ASSIGN;
                break;
            case Token::Tag::Variable:
                de = Synonym::DesignEntity::VARIABLE;
                break;
            case Token::Tag::Constant:
                de = Synonym::DesignEntity::CONSTANT;
                break;
            case Token::Tag::Procedure:
                de = Synonym::DesignEntity::PROCEDURE;
                break;
            default:
                break;
        }
        pqlTokenScanner.next();
        return de;
    }
}

void DeclarationParser::parseDeclarations(Synonym::DesignEntity de) {
    while (true) {
        std::string synonym = parseSynonym();

        if (!SemanticValidator::isDeclared(synonyms, synonym)) {
            synonyms.insert({synonym, de});
        } else {
            throw SemanticException();
        }

        if (pqlTokenScanner.match(Token::Tag::SemiColon)) {
            break;
        } else if (pqlTokenScanner.match(Token::Tag::Comma)) {
            continue;
        }
    }
}

std::string DeclarationParser::parseSynonym() {
    std::string synonym;
    synonym = pqlTokenScanner.peekLexeme();
    pqlTokenScanner.next();
    return synonym;
}
