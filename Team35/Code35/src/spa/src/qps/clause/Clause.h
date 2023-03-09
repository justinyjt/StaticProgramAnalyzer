#pragma once

#include <memory>
#include <utility>

#include "qps/result/Result.h"
#include "qps/result/TableResult.h"
#include "qps/result/BoolResult.h"
#include "pkb/PKBReader.h"
#include "commons/types.h"

/* 
Base class for clause in PQL (suchthat-cl, pattern-cl)
            Clause
            /     \
   SelectClause   TwoArgClause
                  /         \
            AssignPattern  Modifies/Uses/Parent/Follows
*/
class Clause {
 public:
    virtual ~Clause() = default;
    virtual bool operator==(const Clause &rhs) const = 0;
    bool operator!=(const Clause &rhs) const;

    virtual std::unique_ptr<Result> evaluate(PKBReader *db) = 0;
};
