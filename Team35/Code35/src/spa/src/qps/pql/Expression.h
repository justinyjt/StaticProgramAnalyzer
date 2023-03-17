#pragma once
#include "string"
#include "PQLToken.h"
#include "commons/types.h"

/*
"<expr>, _<expr>_, _"
*/ 
class Expression : public PQLToken {
 public:
    explicit Expression(ASSIGN_PAT_RIGHT exprNode, bool hasWildcard);
    const ASSIGN_PAT_RIGHT exprNode;
    std::string str() const override;
    bool operator==(const PQLToken& rhs) const override;
    bool hasWildcard;
};
