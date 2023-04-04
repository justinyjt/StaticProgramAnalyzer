#pragma once
#include <memory>
#include <utility>

#include "TwoArgClause.h"

class StmtEntClause : public TwoArgClause {
 public:
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    const StmtNameRelationship rs;
    StmtEntClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, StmtNameRelationship);
    std::unique_ptr<Result> evaluate(PKBReader *) override;

 protected:
    std::unique_ptr<Result> handleNumSyn(PKBReader *db, PQLNumber &first, Synonym &second);

    std::unique_ptr<Result> handleNumIdent(PKBReader *db, PQLNumber &first, Ident &second);

    std::unique_ptr<Result> handleNumWc(PKBReader *db, PQLNumber &first);

    std::unique_ptr<Result> handleSynSyn(PKBReader *db, Synonym &first, Synonym &second);

    std::unique_ptr<Result> handleSynWc(PKBReader *db, Synonym &first);

    std::unique_ptr<Result> handleSynIdent(PKBReader *db, Synonym &first, Ident &second);

    bool operator==(const Clause &rhs) const override;
};

class UsesS : public StmtEntClause {
 public:
    UsesS(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    void validateArgs() override;
};

class ModifiesS : public StmtEntClause {
 public:
    ModifiesS(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    void validateArgs() override;
};
