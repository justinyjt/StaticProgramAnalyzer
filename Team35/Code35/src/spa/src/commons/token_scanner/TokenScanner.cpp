#include "TokenScanner.h"

#include <utility>

TokenScanner::TokenScanner(std::unique_ptr<ILexer> lex) : lex_(std::move(lex)), token_lst_() {
    initialise();
}

void TokenScanner::next() {
    ++cur_idx_;
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
};

int TokenScanner::isOffsetValid(uint32_t offset) const {
    return (cur_idx_ + offset < token_lst_.size());
};

void TokenScanner::reset() {
    cur_idx_ = 0;
}

TokenLst TokenScanner::getTokenLst() const {
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
    token_lst_ = getTokenLst();
}
