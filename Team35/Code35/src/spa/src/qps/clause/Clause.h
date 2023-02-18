#pragma once

#include <memory>
#include "qps/result/Result.h"
#include "pkb/PKBReader.h"
#include "commons/types.h"
#include "qps/pql/PQLToken.h"

/* 
Base class for clause in PQL (suchthat-cl, pattern-cl)
            Clause
            /     \
   SelectClause   TwoArgClause
                  /         \
            Pattern  Modifies/Uses/Parent/Follows
*/
class Clause {
 public:
    virtual bool operator==(const Clause &rhs) const = 0;
    bool operator!=(const Clause &rhs) const;

    virtual std::unique_ptr<Result> evaluate(PKBReader* db) = 0;
};
