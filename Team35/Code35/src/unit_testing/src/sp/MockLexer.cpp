#include "MockLexer.h"

MockLexer::MockLexer(const std::vector<Token>& tokens) : tokenLst_(tokens) {}

std::unique_ptr<Token> MockLexer::scan() {
    Token cur = this->tokenLst_.back();
    this->tokenLst_.pop_back();
    return std::make_unique<Token>(cur);
}


