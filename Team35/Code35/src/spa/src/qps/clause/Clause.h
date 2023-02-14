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
    const Tok* first;
    const Tok* second;

    Clause(Tok* first, Tok* second);
    virtual bool operator==(const Clause &rhs) const = 0;
    bool operator!=(const Clause &rhs) const;

    virtual Result* evaluate(PKBReader* db) = 0;
 protected:
    bool equal(const Clause& rhs) const;
};
