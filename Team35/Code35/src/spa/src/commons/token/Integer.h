#pragma once

#include <string>

#include "Token.h"

class Integer : public Token {
 public:
    explicit Integer(int integer_lexeme);
    Integer(int integer_lexeme, Token::Tag tag);
    Integer(int integer_lexeme, LineNumber line_number);
    Integer(int integer_lexeme, Token::Tag tag, LineNumber line_number);
    int GetInteger() const;

 private:
    int integer_lexeme_;
};
