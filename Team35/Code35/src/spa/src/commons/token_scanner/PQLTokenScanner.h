#pragma once

#include "TokenScanner.h"

class PQLTokenScanner : public TokenScanner {
public:
    explicit PQLTokenScanner(std::unique_ptr<ILexer> lex);
    explicit PQLTokenScanner(TokenLst token_lst);

    int peekDesignEntity() const;
    void matchAndValidate(Token::Tag tag);
};
