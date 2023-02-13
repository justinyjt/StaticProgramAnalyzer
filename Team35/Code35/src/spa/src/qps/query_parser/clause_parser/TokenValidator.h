#pragma once

#include <memory>
#include <string>
#include <utility>

#include "commons/lexer/ILexer.h"

class TokenValidator {
 public:
    explicit TokenValidator(std::unique_ptr<ILexer> &lexer);
    std::unique_ptr<Token> validateAndConsumeDesignEntityToken();
    std::unique_ptr<Token> validateAndConsumeSynonymToken();
    std::unique_ptr<Token> validateAndConsumeRelationship();
    std::unique_ptr<Token> validateAndConsumeRelationshipArg();
    std::unique_ptr<Token> validateAndConsumePatternFirstArg();
    std::unique_ptr<Token> validateAndConsumePatternSecondArg();
    void validateAndConsumeTokenType(Token::Tag tag);
    bool isNextTokenType(Token::Tag tag);
    void consumeNextToken();
    bool isEof();

 private:
    std::unique_ptr<ILexer> &lexer;
    std::unique_ptr<Token> cur_;
    bool isName(std::string input);
};

