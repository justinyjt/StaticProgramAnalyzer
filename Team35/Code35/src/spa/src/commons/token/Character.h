#pragma once

#include <string>

#include "Token.h"

class Character : public Token {
 public:
    Character(char ascii_lexeme, LineNumber line_number);
    Character(char ascii_lexeme, Tag tag, LineNumber line_number);
    char GetCharacter();

 private:
    char ascii_lexeme_;
};
