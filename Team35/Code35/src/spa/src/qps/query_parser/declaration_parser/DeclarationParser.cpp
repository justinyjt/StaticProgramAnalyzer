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
        pqlTokenScanner.next();

        // check if there are multiple declarations of the same type
        parseDeclarations(de);
    }
    return synonyms;
}

// convert Token tag to Synonym design entity
Synonym::DesignEntity DeclarationParser::parseDesignEntity() {
    switch (pqlTokenScanner.peekTag()) {
        case Token::Tag::Statement:
            return Synonym::DesignEntity::STMT;
        case Token::Tag::Read:
            return Synonym::DesignEntity::READ;
        case Token::Tag::Print:
            return Synonym::DesignEntity::PRINT;
        case Token::Tag::Call:
            return Synonym::DesignEntity::CALL;
        case Token::Tag::While:
            return Synonym::DesignEntity::WHILE;
        case Token::Tag::If:
            return Synonym::DesignEntity::IF;
        case Token::Tag::Assign:
            return Synonym::DesignEntity::ASSIGN;
        case Token::Tag::Variable:
            return Synonym::DesignEntity::VARIABLE;
        case Token::Tag::Constant:
            return Synonym::DesignEntity::CONSTANT;
        case Token::Tag::Procedure:
            return Synonym::DesignEntity::PROCEDURE;
        default:
            throw std::runtime_error("Design Entity not found");
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
        }

        if (pqlTokenScanner.match(Token::Tag::Comma)) {
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
