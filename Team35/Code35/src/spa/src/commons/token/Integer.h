#pragma once

#include <string>

#include "Token.h"

class Integer : public Token {
 public:
    explicit Integer(const Lexeme &integer_lexeme);
    Integer(const Lexeme &integer_lexeme, Token::Tag tag);
    Integer(const Lexeme &integer_lexeme, LineNumber line_number);
    Integer(const Lexeme &integer_lexeme, Token::Tag tag, LineNumber line_number);
};
