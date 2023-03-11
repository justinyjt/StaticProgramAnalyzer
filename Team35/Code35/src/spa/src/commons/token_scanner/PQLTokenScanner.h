#pragma once

#include <memory>
#include <string>
#include <utility>

#include "TokenScanner.h"
#include "qps/query_exceptions/SyntaxException.h"

class PQLTokenScanner : public TokenScanner {
 public:
    explicit PQLTokenScanner(std::unique_ptr<ILexer> lex);
    explicit PQLTokenScanner(TokenLst token_lst);
    int peekDesignEntity() const;
    int peekIdent() const;
    int peekConstant() const;
    int peekSynonym() const;
    bool peekStmtRef();
    bool peekEntRef();
    void matchAndValidate(Token::Tag tag);
};
