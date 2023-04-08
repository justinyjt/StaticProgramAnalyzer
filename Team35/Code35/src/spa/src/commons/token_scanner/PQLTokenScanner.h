#pragma once

#include <memory>
#include <string>
#include <utility>

#include "TokenScanner.h"
#include "qps/query_exceptions/SyntaxException.h"

class PQLTokenScanner : public TokenScanner {
 public:
    explicit PQLTokenScanner(std::unique_ptr<ILexer> lex);
    int peekDesignEntity() const;
    int peekSuchThatClause() const;
    int peekIdent() const;
};
