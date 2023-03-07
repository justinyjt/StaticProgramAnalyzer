#pragma once
#include <memory>
#include <utility>

#include "TwoArgClause.h"

class StmtNameClause : public TwoArgClause {
 public:
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    const StmtNameRelationship rs;
    StmtNameClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, StmtNameRelationship);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
 protected:
    bool operator==(const Clause &rhs) const override;
};

class UsesS : public StmtNameClause {
 public:
    UsesS(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    void validateArgs() override;
};

class ModifiesS : public StmtNameClause {
 public:
    ModifiesS(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    void validateArgs() override;
};
