#include "Lexer.h"

#include <cctype>
#include <utility>

#include "commons/lexer/exception/LexerException.h"
#include "commons/token/Integer.h"

Lexer::Lexer::Builder &Lexer::Lexer::Builder::setIncludeOperator(bool include_operator) {
    include_operator_ = include_operator;
    return *this;
}

Lexer::Lexer::Builder &Lexer::Lexer::Builder::setIncludeString(bool include_string) {
    include_string_ = include_string;
    return *this;
}

Lexer::Lexer::Builder &Lexer::Lexer::Builder::setSource(const Source &source) {
    source_ = source;
    return *this;
}

Lexer::Lexer::Builder &Lexer::Lexer::Builder::addKeyword(const Keyword &keyword) {
    keyword_list_.push_back(keyword);
    return *this;
}

Lexer::Lexer::Builder &Lexer::Lexer::Builder::addCharacter(const Character &character) {
    character_list_.push_back(character);
    return *this;
}

std::unique_ptr<Lexer> Lexer::Lexer::Builder::build() {
    return std::make_unique<Lexer>(source_, keyword_list_, character_list_, include_operator_, include_string_);
}

Lexer::Lexer(Source source,
             const KeywordList &keyword_list,
             const CharacterList &character_list,
             bool include_operator,
             bool include_string)
    : source_(std::move(source)),
      keyword_map_(),
      character_map_(),
      include_operator_(include_operator),
      include_string_(include_string) {
    for (const auto &keyword : keyword_list) {
        reserve(keyword);
    }
    for (const auto &character : character_list) {
        reserve(character);
    }
}

void Lexer::reset() {
    current_position_ = 0;
    current_line_ = 1;
}

std::unique_ptr<Token> Lexer::scan() {
    if (isEof()) {
        return std::make_unique<Token>(Token::Tag::EndOfFile, getCurrentLineNumber());
    }
    while (!isEof() && isControlOrSpace(peekChar())) {
        skipChar();
    }

    if (isEof()) {
        return std::make_unique<Token>(Token::Tag::EndOfFile, getCurrentLineNumber());
    }

    if (include_operator_) {
        char character = readChar();
        switch (character) {
            case '&':
                if (readChar('&')) {
                    return std::make_unique<Token>("&&", Token::Tag::LogicalAnd, getCurrentLineNumber());
                }
                break;
            case '|':
                if (readChar('|')) {
                    return std::make_unique<Token>("||", Token::Tag::LogicalOr, getCurrentLineNumber());
                }
                break;
            case '=':
                if (readChar('=')) {
                    return std::make_unique<Token>("==", Token::Tag::Equivalence, getCurrentLineNumber());
                }
                break;
            case '!':
                if (readChar('=')) {
                    return std::make_unique<Token>("!=", Token::Tag::NotEqual, getCurrentLineNumber());
                }
                return std::make_unique<Character>(character, Token::Tag::LogicalNot, getCurrentLineNumber());
            case '<':
                if (readChar('=')) {
                    return std::make_unique<Token>("<=", Token::Tag::LessThanEqualTo, getCurrentLineNumber());
                }
                return std::make_unique<Character>('<', Token::Tag::LessThan, getCurrentLineNumber());
            case '>':
                if (readChar('=')) {
                    return std::make_unique<Token>(">=", Token::Tag::GreaterThanEqualTo, getCurrentLineNumber());
                }
                return std::make_unique<Character>('>', Token::Tag::GreaterThan, getCurrentLineNumber());
            default:
                break;
        }
        unreadChar();
    }

    char character = peekChar();

    if (isDigit(character)) {
        return scanNextInteger();
    }

    if (isNameStart(character)) {
        return scanNextName();
    }

    if (include_string_ && isStringStartEnd(character)) {
        return scanNextString();
    }

    return scanNextCharacter();
}

LineNumber Lexer::getCurrentLineNumber() const {
    return current_line_;
}

char Lexer::readChar() {
    char c = peekChar();
    skipChar();
    return c;
}

bool Lexer::readChar(char c) {
    if (peekChar() != c) {
        return false;
    }
    skipChar();
    return true;
}

char Lexer::peekChar() {
    return source_[current_position_];
}

void Lexer::unreadChar() {
    if (current_position_ == 0) {
        return;
    }
    current_position_--;
    if (isNewLine(peekChar())) {
        current_line_--;
    }
}

void Lexer::skipChar() {
    if (isNewLine(peekChar())) {
        current_line_++;
    }
    current_position_++;
}

void Lexer::reserve(const Keyword &keyword) {
    keyword_map_.insert(std::make_pair(keyword.getLexeme(), keyword));
}

void Lexer::reserve(const Character &character) {
    character_map_.insert(std::make_pair(character.getCharacter(), character));
}

bool Lexer::isEof() const {
    return current_position_ >= source_.length();
}

std::unique_ptr<Token> Lexer::scanNextName() {
    std::string lexeme;
    lexeme += readChar();
    while (!isEof() && isNamePart(peekChar())) {
        lexeme += readChar();
    }
    if (keyword_map_.find(lexeme) != keyword_map_.end()) {
        return std::make_unique<Keyword>(keyword_map_.at(lexeme), getCurrentLineNumber());
    }
    return std::make_unique<Token>(lexeme, Token::Tag::Name, getCurrentLineNumber());
}

std::unique_ptr<Token> Lexer::scanNextInteger() {
    Lexeme integer_lexeme;
    while (!isEof() && isDigit(peekChar())) {
        integer_lexeme += readChar();
    }
    return std::make_unique<Integer>(integer_lexeme, getCurrentLineNumber());
}

std::unique_ptr<Token> Lexer::scanNextCharacter() {
    char character = readChar();
    if (character_map_.find(character) == character_map_.end()) {
        throw LexerException("Invalid character", getCurrentLineNumber());
    }
    return std::make_unique<Character>(character_map_.at(character), getCurrentLineNumber());
}

std::unique_ptr<Token> Lexer::scanNextString() {
    skipChar();
    std::string lexeme;
    while (!isEof() && !isStringStartEnd(peekChar())) {
        lexeme += readChar();
    }
    if (isEof()) {
        throw LexerException("Unterminated string", getCurrentLineNumber());
    }
    skipChar();
    return std::make_unique<Token>(lexeme, Token::Tag::String, getCurrentLineNumber());
}

bool Lexer::isNameStart(char c) const {
    return isalpha(c);
}

bool Lexer::isNamePart(char c) const {
    return isalnum(c);
}

bool Lexer::isDigit(char c) const {
    return isdigit(c);
}

bool Lexer::isStringStartEnd(char c) const {
    return c == '"';
}

bool Lexer::isNewLine(char c) const {
    return c == '\n';
}

bool Lexer::isControlOrSpace(char c) const {
    return iscntrl(c) || isspace(c);
}

bool Lexer::operator==(const Lexer &rhs) const {
    return this->source_ == rhs.source_ &&
        this->current_position_ == rhs.current_position_ &&
        this->current_line_ == rhs.current_line_ &&
        this->keyword_map_ == rhs.keyword_map_ &&
        this->character_map_ == rhs.character_map_ &&
        this->include_operator_ == rhs.include_operator_ &&
        this->include_string_ == rhs.include_string_;
}

bool Lexer::operator!=(const Lexer &rhs) const {
    return !(rhs == *this);
}
