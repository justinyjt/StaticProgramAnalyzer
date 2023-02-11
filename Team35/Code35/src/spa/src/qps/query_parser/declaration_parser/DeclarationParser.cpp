#include "DeclarationParser.h"

#include <utility>

#include "commons/token/Token.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "qps/query_exceptions/SyntaxException.h"

std::vector<Synonym> DeclarationParser::parse(TokenValidator &tokenValidator) {
    std::vector<Synonym> declarationList;

    // check for next declaration
    while (true) {
        if (tokenValidator.isNextTokenType(Token::Tag::Select)) {
            break;
        }

        std::unique_ptr<Token> designEntity = tokenValidator.validateAndConsumeDesignEntityToken();
        Synonym::DesignEntity de = processDesignEntity(std::move(designEntity));

        // check if there are multiple declarations of the same type
        while (true) {
            std::unique_ptr<Token> synonym = tokenValidator.validateAndConsumeSynonymToken();
            if (!isDeclared(synonym->getLexeme(), declarationList)) {
                declarationList.push_back(Synonym(de, synonym->getLexeme()));
            } else {
                throw SyntaxException();
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
Synonym::DesignEntity DeclarationParser::processDesignEntity(std::unique_ptr<Token> token) {
    if (token->getTag() == Token::Tag::Statement) {
        return Synonym::DesignEntity::STMT;
    } else if (token->getTag() == Token::Tag::Read) {
        return Synonym::DesignEntity::READ;
    } else if (token->getTag() == Token::Tag::Print) {
        return Synonym::DesignEntity::PRINT;
    } else if (token->getTag() == Token::Tag::Call) {
        return Synonym::DesignEntity::CALL;
    } else if (token->getTag() == Token::Tag::While) {
        return Synonym::DesignEntity::WHILE;
    } else if (token->getTag() == Token::Tag::If) {
        return Synonym::DesignEntity::IF;
    } else if (token->getTag() == Token::Tag::Assign) {
        return Synonym::DesignEntity::ASSIGN;
    } else if (token->getTag() == Token::Tag::Variable) {
        return Synonym::DesignEntity::VARIABLE;
    } else if (token->getTag() == Token::Tag::Constant) {
        return Synonym::DesignEntity::CONSTANT;
    } else if (token->getTag() == Token::Tag::Procedure) {
        return Synonym::DesignEntity::PROCEDURE;
    }
}

bool DeclarationParser::isDeclared(std::string value, std::vector<Synonym> declarationList) {
    bool isDeclared = false;
    for (Synonym s : declarationList) {
        if (value == s.getValue()) {
            isDeclared = true;
        }
    }
    return isDeclared;
}
