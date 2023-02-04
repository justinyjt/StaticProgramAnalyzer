#pragma once

#include "Token.h"

class Keyword : public Token {
 public:
    explicit Keyword(Lexeme lexeme);
    Keyword(Lexeme lexeme, Tag tag);
    Keyword(Keyword &keyword, LineNumber line_number);
    Keyword(Lexeme lexeme, LineNumber line_number);
    Keyword(Lexeme lexeme, Tag tag, LineNumber line_number);
};
