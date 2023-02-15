#pragma once
#include "qps/result/Result.h"
#include "pkb/PKBReader.h"
#include "commons/types.h"
#include "qps/pql/PQLToken.h"

/* 
Base class for select clause in PQL (suchthat-cl, pattern-cl)
*/
class Clause {
 public:
    const PQLToken* first;
    const PQLToken* second;

    Clause(PQLToken* first, PQLToken* second);
    virtual bool operator==(const Clause &rhs) const = 0;
    bool operator!=(const Clause &rhs) const;

    virtual Result* evaluate(PKBReader* db) = 0;
 protected:
    bool equal(const Clause& rhs) const;
    int caseValue() const;
    static constexpr int c(PQLToken::Tag t1, PQLToken::Tag t2) {
      return 10 * static_cast<int>(t1) + static_cast<int>(t2);
    }
};
