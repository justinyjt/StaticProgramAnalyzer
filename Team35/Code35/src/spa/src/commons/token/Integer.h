#pragma once

#include "Token.h"

class Integer : public Token {
 public:
    explicit Integer(const Lexeme &integer_lexeme);
    Integer(const Lexeme &integer_lexeme, Token::Tag tag);
};
