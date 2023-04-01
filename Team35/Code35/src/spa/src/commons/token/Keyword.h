#pragma once

#include "Token.h"

class Keyword : public Token {
 public:
    explicit Keyword(Lexeme lexeme);
    Keyword(Lexeme lexeme, Tag tag);
};
