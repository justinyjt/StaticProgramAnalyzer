#include "Integer.h"

Integer::Integer(const Lexeme &integer_lexeme) : Token::Token(integer_lexeme, Token::Tag::Integer) {}

Integer::Integer(const Lexeme &integer_lexeme, Token::Tag tag) : Token::Token(integer_lexeme, tag) {}

Integer::Integer(const Lexeme &integer_lexeme, LineNumber line_number) : Token::Token(integer_lexeme,
                                                                                      Token::Tag::Integer,
                                                                                      line_number) {}

Integer::Integer(const Lexeme &integer_lexeme, Token::Tag tag, LineNumber line_number) : Token::Token(integer_lexeme,
                                                                                                      tag,
                                                                                                      line_number) {}
