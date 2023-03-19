#pragma once
#include <memory>
#include <utility>

#include "TwoArgClause.h"

class StmtStmtClause : public TwoArgClause {
    /* <SYNONYM | _ | STMT_NUM> */
 public:
    const StmtStmtRelationship rs;
    StmtStmtClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, StmtStmtRelationship);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
 protected:
    bool operator==(const Clause& rhs) const override;
};

class Parent : public StmtStmtClause {
 public:
    Parent(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, bool isTransitive);
};


class Follows : public StmtStmtClause {
 public:
    Follows(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, bool isTransitive);
};

class Next : public StmtStmtClause {
 public:
    Next(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, bool isTransitive);
};


class Affects : public StmtStmtClause {
 public:
    Affects(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, bool isTransitive);
};
