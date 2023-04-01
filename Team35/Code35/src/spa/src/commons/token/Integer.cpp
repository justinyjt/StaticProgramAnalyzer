#include "Integer.h"

Integer::Integer(const Lexeme &integer_lexeme) : Token::Token(integer_lexeme, Token::Tag::Integer) {}

Integer::Integer(const Lexeme &integer_lexeme, Token::Tag tag) : Token::Token(integer_lexeme, tag) {}
