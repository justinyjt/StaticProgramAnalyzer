#include "TokenValidator.h"

TokenValidator::TokenValidator(std::unique_ptr<ILexer> &lexer) : lexer(lexer) {}

std::unique_ptr<Token> TokenValidator::getNextToken() {
    return lexer->scan();
}