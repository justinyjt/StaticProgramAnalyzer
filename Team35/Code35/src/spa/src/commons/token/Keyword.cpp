#include "Keyword.h"

#include <utility>

Keyword::Keyword(Lexeme lexeme, Token::Tag tag) : Token(std::move(lexeme), tag) {}

Keyword::Keyword(Lexeme lexeme) : Token(std::move(lexeme), Tag::Keyword) {}
