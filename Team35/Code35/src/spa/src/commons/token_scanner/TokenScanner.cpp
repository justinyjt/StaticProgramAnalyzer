#include "TokenScanner.h"

#include <utility>

TokenScanner::TokenScanner(std::unique_ptr<ILexer> lex) : lex_(std::move(lex)), token_lst_() {
    initialise();
    token_lst_ = getTokenLst();
}

TokenScanner::TokenScanner(TokenLst token_lst) : lex_(nullptr), token_lst_(std::move(token_lst)) {
    initialise();
}

/**
 * Increment the current index by 1, and return 1 if the current index is incremented and still valid.
 * @return
 */
int TokenScanner::next() {
    if (cur_idx_ + 1 >= token_lst_.size()) {
        return 0;
    }
    ++cur_idx_;
    return 1;
}

int TokenScanner::peek(Token::Tag tag) const {
    return token_lst_[cur_idx_]->getTag() == tag;
}

int TokenScanner::match(Token::Tag tag) {
    if (peek(tag)) {
        if (tag != Token::Tag::EndOfFile) {
            next();
        }
        return 1;
    }
    return 0;
}

int TokenScanner::peekOffset(Token::Tag tag, uint32_t offset) const {
    if (!isOffsetValid(offset)) {
        return false;
    }
    return (token_lst_[cur_idx_ + offset]->getTag() == tag);
}

Lexeme TokenScanner::peekLexeme() const {
    return token_lst_[cur_idx_]->getLexeme();
}

int TokenScanner::isOffsetValid(uint32_t offset) const {
    return (cur_idx_ + offset < token_lst_.size());
}

void TokenScanner::reset() {
    initialise();
}

/**
 * If the TokenScanner is constructed with a TokenLst, this method can only be called once.
 * @return the TokenLst.
 */
TokenLst TokenScanner::getTokenLst() {
    if (lex_ == nullptr) {
        TokenLst token_lst = std::move(token_lst_);
        token_lst_ = TokenLst();
        token_lst_.push_back(std::make_unique<Token>(Token::Tag::EndOfFile));
        return std::move(token_lst);
    }
    lex_->reset();
    TokenLst token_lst;
    std::unique_ptr<Token> cur = lex_->scan();
    while (cur->getTag() != Token::Tag::EndOfFile) {
        token_lst.push_back(std::move(cur));
        cur = lex_->scan();
    }
    token_lst.push_back(std::move(cur));
    return std::move(token_lst);
}

void TokenScanner::initialise() {
    cur_idx_ = 0;
}
