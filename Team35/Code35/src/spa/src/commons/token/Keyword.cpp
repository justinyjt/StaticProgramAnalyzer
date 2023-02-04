#include "Keyword.h"

#include <utility>

Keyword::Keyword(Lexeme lexeme, LineNumber line_number) : Token(std::move(lexeme), Tag::Keyword, line_number) {}

Keyword::Keyword(Lexeme lexeme, Tag tag, LineNumber line_number) : Token(std::move(lexeme), tag, line_number) {}

Keyword::Keyword(Lexeme lexeme, Token::Tag tag) : Token(std::move(lexeme), tag) {}

Keyword::Keyword(Lexeme lexeme) : Token(std::move(lexeme), Tag::Keyword) {}

Keyword::Keyword(const Keyword &keyword, LineNumber line_number) : Token(keyword, line_number) {}
