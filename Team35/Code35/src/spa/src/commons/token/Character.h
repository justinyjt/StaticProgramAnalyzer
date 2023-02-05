#pragma once

#include <string>

#include "Token.h"

class Character : public Token {
 public:
    explicit Character(char ascii_lexeme);
    Character(char ascii_lexeme, Tag tag);
    Character(char ascii_lexeme, LineNumber line_number);
    Character(const Character &character, LineNumber line_number);
    Character(char ascii_lexeme, Tag tag, LineNumber line_number);
    char getCharacter() const;

 private:
    char ascii_lexeme_;
};
