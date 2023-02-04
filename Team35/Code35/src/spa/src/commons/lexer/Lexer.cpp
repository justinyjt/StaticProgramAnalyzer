#include "Lexer.h"

#include <cctype>
#include <utility>

#include "commons/token/Integer.h"
#include "commons/util/ConversionUtil.h"

Lexer::Lexer(Source source, KeywordList keyword_list, CharacterList character_list)
    : source_(source), keyword_map_(), character_map_() {
    for (auto keyword : keyword_list) {
        keyword_map_.insert(std::make_pair(keyword.GetLexeme(), keyword));
    }
    for (auto character : character_list) {
        character_map_.insert(std::make_pair(character.GetCharacter(), character));
    }
}

Token Lexer::Scan() {
    if (IsEOF()) {
        return Token(Token::Tag::EndOfFile, current_line_);
    }
    char character = PeekChar();
    while (true) {
        if (IsEOF()) {
            return Token(Token::Tag::EndOfFile, current_line_);
        } else if (IsNewLine(character)) {
            current_line_++;
        } else if (IsControlOrSpace(character)) {
            continue;
        } else {
            break;
        }
        character = ReadChar();
    }

    switch (character) {
        case '&':
            if (ReadChar('&')) {
                return Token("&&", Token::Tag::LogicalAnd, current_line_);
            }
        case '|':
            if (ReadChar('|')) {
                return Token("||", Token::Tag::LogicalOr, current_line_);
            }
        case '=':
            if (ReadChar('=')) {
                return Token("==", Token::Tag::Equivalence, current_line_);
            }
        case '!':
            if (ReadChar('=')) {
                return Token("!=", Token::Tag::NotEqual, current_line_);
            }
        case '<':
            if (ReadChar('=')) {
                return Token("<=", Token::Tag::LessThanEqualTo, current_line_);
            } else {
                return Token("<", Token::Tag::LessThan, current_line_);
            }
        case '>':
            if (ReadChar('=')) {
                return Token(">=", Token::Tag::GreaterThanEqualTo, current_line_);
            } else {
                return Token(">", Token::Tag::GreaterThan, current_line_);
            }
        default:break;
    }

    if (IsDigit(character)) {
        return ScanNextInteger(character);
    } else if (IsNameStart(character)) {
        return ScanNextName(character);
    } else if (IsDoubleQuotes(character)) {
        return ScanNextString();
    } else {
        return ScanNextCharacter(character);
    }
}

char Lexer::ReadChar() {
    return source_[current_position_++];
}

bool Lexer::ReadChar(char c) {
    if (PeekChar() != c) {
        return false;
    }
    current_position_++;
    return true;
}

char Lexer::PeekChar() {
    return source_[current_position_];
}

char Lexer::PeekNextChar() {
    return source_[current_position_ + 1];
}

void Lexer::SkipChar() {
    current_position_++;
}

Token Lexer::ScanNextName(char first_char) {
    std::string lexeme;
    lexeme += first_char;
    while (!IsEOF() && IsNamePart(PeekChar())) {
        lexeme += ReadChar();
    }
    if (keyword_map_.find(lexeme) != keyword_map_.end()) {
        return Keyword(keyword_map_.at(lexeme), current_line_);
    }
    return Token(lexeme, Token::Tag::Name, current_line_);
}

Token Lexer::ScanNextInteger(char first_char) {
    int number = 0 + ConvertCharToInt(first_char);
    while (!IsEOF() && IsDigit(PeekChar())) {
        number *= 10;
        number += ConvertCharToInt(ReadChar());
    }
    return Integer(number, current_line_);
}

Token Lexer::ScanNextCharacter(char first_char) {
    char character = first_char;
    if (character_map_.find(character) == character_map_.end()) {
        throw std::runtime_error("Invalid character");
    }
    return Character(character_map_.at(character), current_line_);
}

Token Lexer::ScanNextString() {
    SkipChar();
    std::string lexeme;
    while (!IsEOF() && !IsDoubleQuotes(PeekChar())) {
        lexeme += ReadChar();
    }
    if (IsEOF()) {
        throw std::runtime_error("Unterminated string");
    }
    SkipChar();
    return Token(lexeme, Token::Tag::String, current_line_);
}

bool Lexer::IsNameStart(char c) {
    return isalpha(c);
}

bool Lexer::IsNamePart(char c) {
    return isalnum(c);
}

bool Lexer::IsDigit(char c) {
    return isdigit(c);
}

bool Lexer::IsDoubleQuotes(char c) {
    return c == '"';
}

bool Lexer::IsNewLine(char c) {
    return c == '\n';
}

bool Lexer::IsControlOrSpace(char c) {
    return iscntrl(c) || isspace(c);
}

bool Lexer::IsEOF() {
    return current_position_ >= source_.length();
}
