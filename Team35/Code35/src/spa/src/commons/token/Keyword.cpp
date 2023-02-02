#include "Keyword.h"

Keyword::Keyword(Lexeme lexeme, LineNumber line_number) : Token(lexeme, Tag::Keyword, line_number) {}

Keyword::Keyword(Lexeme lexeme, Tag tag, LineNumber line_number) : Token(lexeme, tag, line_number) {}
