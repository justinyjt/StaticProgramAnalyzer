#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "TwoArgClause.h"

class WithEntClause : public TwoArgClause {
 public:
    /* <stmt SYNONYM | name SYNONYM | NUM | ENT>, <stmt SYNONYM | name SYNONYM | NUM | ENT> */
    WithEntClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);

    std::unique_ptr<Result> evaluate(PKBReader *) override;

    void validateArgs() override;

 protected:
    static STMT_ENT_SET getStmtEntSet(PKBReader *db, Synonym &syn);

    static STMT_SET getStmtSet(PKBReader *db, Synonym &syn);

    static ENT_SET getEntSet(PKBReader *db, Synonym &syn);

    static bool isStmtSyn(Synonym &syn);

    static std::unique_ptr<Result> handleStmtStmt(PKBReader *db, Synonym &syn1, Synonym &syn2);

    static std::unique_ptr<Result> handleStmtEnt(PKBReader *db, Synonym &stmtSyn, Synonym &entSyn);

    static std::unique_ptr<Result> handleEntEnt(PKBReader *db, Synonym &syn1, Synonym &syn2);

    static std::unique_ptr<Result> handleSynSyn(PKBReader *db, Synonym &syn1, Synonym &syn2);

    static std::unique_ptr<Result> handleSynEnt(PKBReader *db, Synonym &syn, ENT_NAME &ent);

    static std::unique_ptr<Result> handleSameSynCase(PKBReader *db, Synonym &syn);

    bool operator==(const Clause &rhs) const override;
};
