#pragma once

#include <list>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "commons/token/Character.h"
#include "commons/token/Keyword.h"
#include "ILexer.h"

typedef std::string Source;
typedef std::unordered_map<std::string, Keyword> KeywordMap;
typedef std::unordered_map<char, Character> CharacterMap;
typedef std::list<Keyword> KeywordList;
typedef std::list<Character> CharacterList;

class Lexer : public ILexer {
 public:
    class Builder {
     public:
        Builder() = default;
        Builder &setSource(const Source &source);
        Builder &addKeyword(const Keyword &keyword);
        Builder &addCharacter(const Character &character);
        Builder &setIncludeOperator(bool include_operator);
        Builder &setIncludeString(bool include_string);
        std::unique_ptr<Lexer> build();

     private:
        Source source_;
        KeywordList keyword_list_;
        CharacterList character_list_;
        bool include_operator_ = true;
        bool include_string_ = true;
    };
    Lexer(Source source,
          const KeywordList &keyword_list,
          const CharacterList &character_list,
          bool include_operator = true,
          bool include_string = true);
    ~Lexer() = default;
    bool operator==(const Lexer &rhs) const;
    bool operator!=(const Lexer &rhs) const;
    std::unique_ptr<Token> scan() override;
    void reset() override;

 protected:
    char readChar();
    bool readChar(char c);
    char peekChar();
    void unreadChar();
    void skipChar();
    bool isEof() const;
    void reserve(const Keyword &keyword);
    void reserve(const Character &character);

 private:
    Source source_;
    uint32_t current_position_ = 0;
    KeywordMap keyword_map_;
    CharacterMap character_map_;
    bool include_operator_;
    bool include_string_;

    std::unique_ptr<Token> scanNextName();
    std::unique_ptr<Token> scanNextInteger();
    std::unique_ptr<Token> scanNextString();
    std::unique_ptr<Token> scanNextCharacter();
    std::optional<std::unique_ptr<Token>> scanNextOperator();

    static bool isNameStart(char c);
    static bool isNamePart(char c);
    static bool isDigit(char c);
    static bool isStringStartEnd(char c);
    static bool isControlOrSpace(char c);
};
