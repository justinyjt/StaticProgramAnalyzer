#pragma once

#include <deque>
#include <memory>

#include "commons/lexer/ILexer.h"
#include "commons/token/Token.h"

typedef std::deque<std::unique_ptr<Token>> TokenLst;

class TokenScanner {
 public:
    explicit TokenScanner(std::unique_ptr<ILexer> lex);
    explicit TokenScanner(TokenLst token_lst);
    int next();
    int peek(Token::Tag tag) const;
    int match(Token::Tag tag);
    int peekOffset(Token::Tag tag, uint32_t offset) const;
    Lexeme peekLexeme() const;
    Token::Tag peekTag() const;
    int isOffsetValid(uint32_t offset) const;
    void reset();
    void saveState();
    void restoreState();
    [[nodiscard]] TokenLst getTokenLst();

 private:
    std::unique_ptr<ILexer> lex_;
    std::deque<std::unique_ptr<Token>> token_lst_;
    uint32_t cur_idx_ = 0;
    uint32_t saved_idx_ = 0;
    void initialise();
};
