#include "TokenScanner.h"

#include <utility>

/**
 * Initialise the TokenScanner. Throws LexerException if the lexer detects an invalid token.
 */
TokenScanner::TokenScanner(std::unique_ptr<ILexer> lex) : lex_(std::move(lex)), token_lst_() {
    initialise();
}

/**
 * Initialise the TokenScanner. Deprecated. Do not use.
 */
TokenScanner::TokenScanner(TokenLst token_lst) : lex_(nullptr), token_lst_(std::move(token_lst)) {}

/**
 * Increment the current index by 1.
 * @return return 1 if the current index is incremented and still valid.
 */
int TokenScanner::next() {
    if (cur_idx_ + 1 >= token_lst_.size()) {
        return 0;
    }
    ++cur_idx_;
    return 1;
}

/**
 * Check if the current token has the given tag.
 * @param tag
 * @return 0 if the current token does not have the given tag, 1 otherwise.
 */
int TokenScanner::peek(Token::Tag tag) const {
    return token_lst_[cur_idx_]->getTag() == tag;
}

/**
 * Check if the current token has the given tag. If the current token has the given tag, the current index is
 * incremented by 1.
 * @param tag
 * @return 0 if the current token does not have the given tag, 1 otherwise.
 */
int TokenScanner::match(Token::Tag tag) {
    if (peek(tag)) {
        if (tag != Token::Tag::EndOfFile) {
            next();
        }
        return 1;
    }
    return 0;
}

/**
 * Check if the token at the given offset has the given tag.
 * @param tag
 * @param offset
 * @return 0 if the token at the given offset does not have the given tag or the offset is invalid, 1 otherwise.
 */
int TokenScanner::peekOffset(Token::Tag tag, uint32_t offset) const {
    if (!isOffsetValid(offset)) {
        return false;
    }
    return (token_lst_[cur_idx_ + offset]->getTag() == tag);
}

/**
 * Get the lexeme of the current token.
 * @return the lexeme of the current token.
 */
Lexeme TokenScanner::peekLexeme() const {
    return token_lst_[cur_idx_]->getLexeme();
}

Token::Tag TokenScanner::peekTag() const {
    return token_lst_[cur_idx_]->getTag();
}

int TokenScanner::isName() const {
    // check for names that can also be keywords
    if (peek(Token::Tag::String)) {
        return false;
    }
    std::string input = peekLexeme();
    if (!isalpha(input[0])) {
        return false;
    }
    for (char c : input) {
        if (!isalpha(c) && !isdigit(c)) {
            return false;
        }
    }
    return true;
}

/**
 * Check if the given offset is valid.
 * @param offset
 * @return 0 if the given offset is invalid, 1 otherwise.
 */
int TokenScanner::isOffsetValid(uint32_t offset) const {
    return (cur_idx_ + offset < token_lst_.size());
}

/**
 * Reset the current index to 0.
 */
void TokenScanner::reset() {
    cur_idx_ = 0;
}

/**
 * Save the current index.
 */
void TokenScanner::saveState() {
    saved_idx_ = cur_idx_;
}

/**
 * Restore the current index to the saved index. If the saved index is not set, the current index is set to 0.
 */
void TokenScanner::restoreState() {
    cur_idx_ = saved_idx_;
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

/**
 * Initialise the TokenScanner.
 */
void TokenScanner::initialise() {
    token_lst_ = getTokenLst();
    cur_idx_ = 0;
    saved_idx_ = 0;
}
