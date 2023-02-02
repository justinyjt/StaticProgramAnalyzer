#include "Token.h"

Token::Token(Tag tag, LineNumber line_number) : tag_(tag), line_number_(line_number) {}

Token::Token(Lexeme lexeme, Tag tag, LineNumber line_number) : lexeme_(lexeme), tag_(tag), line_number_(line_number) {}

Lexeme Token::GetLexeme() {
    return lexeme_;
}

Token::Tag Token::GetTag() {
    return tag_;
}

LineNumber Token::GetLineNumber() {
    return line_number_;
}
