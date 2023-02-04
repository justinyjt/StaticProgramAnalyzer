#include "Lexer.h"

#include <cassert>
#include <cctype>
#include <utility>

#include "commons/lexer/exception/LexerException.h"
#include "commons/token/Integer.h"
#include "commons/util/ConversionUtil.h"

Lexer::Lexer(Source source, const KeywordList &keyword_list, const CharacterList &character_list)
    : source_(std::move(source)), keyword_map_(), character_map_() {
    for (const auto &keyword : keyword_list) {
        Reserve(keyword);
    }
    for (const auto &character : character_list) {
        Reserve(character);
    }
}

Token Lexer::Scan() {
    if (IsEOF()) {
        return Token(Token::Tag::EndOfFile, GetCurrentLineNumber());
    }
    char character = PeekChar();
    while (!IsEOF() && IsControlOrSpace(character)) {
        character = ReadChar();
    }

    if (IsEOF()) {
        return Token(Token::Tag::EndOfFile, GetCurrentLineNumber());
    }

    switch (character) {
        case '&':
            if (ReadChar('&')) {
                return Token("&&", Token::Tag::LogicalAnd, GetCurrentLineNumber());
            }
        case '|':
            if (ReadChar('|')) {
                return Token("||", Token::Tag::LogicalOr, GetCurrentLineNumber());
            }
        case '=':
            if (ReadChar('=')) {
                return Token("==", Token::Tag::Equivalence, GetCurrentLineNumber());
            }
        case '!':
            if (ReadChar('=')) {
                return Token("!=", Token::Tag::NotEqual, GetCurrentLineNumber());
            }
        case '<':
            if (ReadChar('=')) {
                return Token("<=", Token::Tag::LessThanEqualTo, GetCurrentLineNumber());
            } else {
                return Token("<", Token::Tag::LessThan, GetCurrentLineNumber());
            }
        case '>':
            if (ReadChar('=')) {
                return Token(">=", Token::Tag::GreaterThanEqualTo, GetCurrentLineNumber());
            } else {
                return Token(">", Token::Tag::GreaterThan, GetCurrentLineNumber());
            }
        default:break;
    }

    UnreadChar();
    character = PeekChar();

    if (IsDigit(character)) {
        return ScanNextInteger();
    }

    if (IsNameStart(character)) {
        return ScanNextName();
    }

    if (IsStringStartEnd(character)) {
        return ScanNextString();
    }

    return ScanNextCharacter();
}

LineNumber Lexer::GetCurrentLineNumber() const {
    return current_line_;
}

char Lexer::ReadChar() {
    char c = PeekChar();
    SkipChar();
    return c;
}

bool Lexer::ReadChar(char c) {
    if (PeekChar() != c) {
        return false;
    }
    SkipChar();
    return true;
}

char Lexer::PeekChar() {
    return source_[current_position_];
}

void Lexer::UnreadChar() {
    if (current_position_ == 0) {
        return;
    }
    current_position_--;
    if (IsNewLine(PeekChar())) {
        current_line_--;
    }
}

void Lexer::SkipChar() {
    if (IsNewLine(PeekChar())) {
        current_line_++;
    }
    current_position_++;
}

void Lexer::Reserve(const Keyword &keyword) {
    keyword_map_.insert(std::make_pair(keyword.GetLexeme(), keyword));
}

void Lexer::Reserve(const Character &character) {
    character_map_.insert(std::make_pair(character.GetCharacter(), character));
}

bool Lexer::IsEOF() const {
    return current_position_ >= source_.length();
}

Token Lexer::ScanNextName() {
    assert(IsNameStart(PeekChar()));
    std::string lexeme;
    lexeme += ReadChar();
    while (!IsEOF() && IsNamePart(PeekChar())) {
        lexeme += ReadChar();
    }
    if (keyword_map_.find(lexeme) != keyword_map_.end()) {
        return Keyword(keyword_map_.at(lexeme), GetCurrentLineNumber());
    }
    return Token(lexeme, Token::Tag::Name, GetCurrentLineNumber());
}

Token Lexer::ScanNextInteger() {
    assert(IsDigit(PeekChar()));
    int number = 0;
    while (!IsEOF() && IsDigit(PeekChar())) {
        number *= 10;
        number += ConvertCharToInt(ReadChar());
    }
    return Integer(number, GetCurrentLineNumber());
}

Token Lexer::ScanNextCharacter() {
    char character = ReadChar();
    if (character_map_.find(character) == character_map_.end()) {
        throw LexerException("Invalid character", GetCurrentLineNumber());
    }
    return Character(character_map_.at(character), GetCurrentLineNumber());
}

Token Lexer::ScanNextString() {
    assert(IsStringStartEnd(PeekChar()));
    SkipChar();
    std::string lexeme;
    while (!IsEOF() && !IsStringStartEnd(PeekChar())) {
        lexeme += ReadChar();
    }
    if (IsEOF()) {
        throw LexerException("Unterminated string", GetCurrentLineNumber());
    }
    SkipChar();
    return Token(lexeme, Token::Tag::String, GetCurrentLineNumber());
}

bool Lexer::IsNameStart(char c) const {
    return isalpha(c);
}

bool Lexer::IsNamePart(char c) const {
    return isalnum(c);
}

bool Lexer::IsDigit(char c) const {
    return isdigit(c);
}

bool Lexer::IsStringStartEnd(char c) const {
    return c == '"';
}

bool Lexer::IsNewLine(char c) const {
    return c == '\n';
}

bool Lexer::IsControlOrSpace(char c) const {
    return iscntrl(c) || isspace(c);
}
