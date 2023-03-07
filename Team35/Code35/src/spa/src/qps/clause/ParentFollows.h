#pragma once
#include <memory>
#include <utility>

#include "TwoArgClause.h"

class ParentFollows : public TwoArgClause {
    /* <SYNONYM | _ | STMT_NUM> */
 public:
    const StmtStmtRelationship rs;
    ParentFollows(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, StmtStmtRelationship);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
 protected:
    bool operator==(const Clause& rhs) const override;
};

class Parent : public ParentFollows {
 public:
    Parent(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, bool isTransitive);
};


class Follows : public ParentFollows {
 public:
    Follows(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, bool isTransitive);
};
