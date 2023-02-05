#include "MockLexer.h"

MockLexer::MockLexer(const std::vector<Token>& tokens) : tokenLst_(tokens) {}

Token MockLexer::scan() {

    Token cur = this->tokenLst_.back();
    this->tokenLst_.pop_back();
    return cur;//Token(Token::Tag::EndOfFile);
}


