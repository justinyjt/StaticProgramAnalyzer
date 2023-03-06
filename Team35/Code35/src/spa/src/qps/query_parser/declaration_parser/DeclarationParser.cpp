#include <utility>

#include "DeclarationParser.h"
#include "commons/token/Token.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_exceptions/SemanticException.h"

std::vector<Synonym> DeclarationParser::parse(TokenValidator& tokenValidator) {
    std::vector<Synonym> declarationList;

    // check for next declaration
    while (true) {
        if (tokenValidator.isNextTokenType(Token::Tag::Select) || tokenValidator.isEof()) {
            break;
        }
        Synonym::DesignEntity de = processDesignEntity(tokenValidator);

        // check if there are multiple declarations of the same type
        while (true) {
            std::unique_ptr<Token> synonym = tokenValidator.validateAndConsumeSynonymToken();
            if (!isDeclared(synonym->getLexeme(), declarationList)) {
                declarationList.emplace_back(de, synonym->getLexeme());
            } else {
                throw SemanticException();
            }

            if (tokenValidator.isNextTokenType(Token::Tag::SemiColon)) {
                tokenValidator.consumeNextToken();
                break;
            } else if (tokenValidator.isNextTokenType(Token::Tag::Comma)) {
                tokenValidator.consumeNextToken();
            } else {
                throw SyntaxException();
            }
        }
    }
    return declarationList;
}

// convert Token tag to Synonym design entity
Synonym::DesignEntity DeclarationParser::processDesignEntity(TokenValidator& tokenValidator) {
    std::unique_ptr<Token> designEntity = tokenValidator.validateAndConsumeDesignEntityToken();
    switch (designEntity->getTag()) {
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
            throw SyntaxException();
    }
}

bool DeclarationParser::isDeclared(const std::string& value, std::vector<Synonym>& declarationList) {
    bool isDeclared = false;
    for (const Synonym& s : declarationList) {
        if (value == s.str()) {
            isDeclared = true;
        }
    }
    return isDeclared;
}
