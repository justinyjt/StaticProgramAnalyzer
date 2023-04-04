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

    static std::unique_ptr<Result> handleStmtStmt(PKBReader *db, Synonym &first, Synonym &second);

    static std::unique_ptr<Result> handleStmtEnt(PKBReader *db, Synonym &first, Synonym &second);

    static std::unique_ptr<Result> handleEntEnt(PKBReader *db, Synonym &first, Synonym &second);

    static std::unique_ptr<Result> handleSynSyn(PKBReader *db, Synonym &first, Synonym &second);

    static std::unique_ptr<Result> handleStmtIdent(PKBReader *db, Synonym &stmt, Ident &ident);

    static std::unique_ptr<Result> handleEntIdent(PKBReader *db, Synonym &ent, Ident &ident);

    static std::unique_ptr<Result> handleSynIdent(PKBReader *db, Synonym &first, Ident &second);

    static std::unique_ptr<Result> handleSameSynCase(PKBReader *db, Synonym &syn);

    bool operator==(const Clause &rhs) const override;
};
