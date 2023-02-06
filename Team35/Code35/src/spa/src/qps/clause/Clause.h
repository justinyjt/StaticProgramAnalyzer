#pragma once
#include "qps/result/Result.h"
#include "pkb/PKBReader.h"
#include "commons/types.h"
#include "qps/pql/Tok.h"

/* 
Base class for select clause in PQL (suchthat-cl, pattern-cl)
*/
class Clause {
 public:
    Tok first;
    Tok second;

    Clause(Tok first, Tok second);
    bool operator==(Clause const &rhs) const;
    virtual Result* evaluate(PKBReader* db) = 0;
};
