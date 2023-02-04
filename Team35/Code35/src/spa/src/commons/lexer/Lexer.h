#pragma once

#include "ILexer.h"

#include <list>
#include <string>
#include <unordered_map>

#include "commons/token/Character.h"
#include "commons/token/Keyword.h"

typedef std::string Source;
typedef std::unordered_map<std::string, Keyword> KeywordMap;
typedef std::unordered_map<char, Character> CharacterMap;
typedef std::list<Keyword> KeywordList;
typedef std::list<Character> CharacterList;

class Lexer : public ILexer {
 public:
    explicit Lexer(Source source, const KeywordList &keyword_list, const CharacterList &character_list);
    Token Scan();
    ~Lexer() = default;

 protected:
    LineNumber GetCurrentLineNumber() const;
    char ReadChar();
    bool ReadChar(char c);
    char PeekChar();
    void UnreadChar();
    void SkipChar();
    bool IsEOF() const;
    void Reserve(const Keyword &keyword);
    void Reserve(const Character &character);

 private:
    Source source_;
    uint32_t current_position_ = 0;
    uint32_t current_line_ = 1;
    KeywordMap keyword_map_;
    CharacterMap character_map_;

    Token ScanNextName();
    Token ScanNextInteger();
    Token ScanNextString();
    Token ScanNextCharacter();

    bool IsNameStart(char c) const;
    bool IsNamePart(char c) const;
    bool IsDigit(char c) const;
    bool IsStringStartEnd(char c) const;
    bool IsNewLine(char c) const;
    bool IsControlOrSpace(char c) const;
};
