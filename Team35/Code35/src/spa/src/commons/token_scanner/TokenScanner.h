#pragma once

#include <memory>
#include <vector>

#include "commons/lexer/ILexer.h"
#include "commons/token/Token.h"

typedef std::vector<std::unique_ptr<Token>> TokenList;

class TokenScanner {
 public:
    explicit TokenScanner(std::unique_ptr<ILexer> lex);
    int next();
    int peek(Token::Tag tag) const;
    int match(Token::Tag tag);
    int peekOffset(Token::Tag tag, uint32_t offset) const;
    Lexeme peekLexeme() const;
    Token::Tag peekTag() const;
    virtual int isName() const;
    int isOffsetValid(uint32_t offset) const;
    void reset();
    void saveState();
    void restoreState();

 private:
    std::unique_ptr<ILexer> lex_;
    TokenList token_list_;
    uint32_t cur_idx_ = 0;
    uint32_t saved_idx_ = 0;
    [[nodiscard]] TokenList getTokenList();
};
