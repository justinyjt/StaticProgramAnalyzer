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
    std::unique_ptr<Result> handleSynSynCase(PKBReader *db, Synonym &first, Synonym &second);

    std::unique_ptr<Result> handleSynNumCase(PKBReader *db, Synonym &first, PQLNumber &second);

    std::unique_ptr<Result> handleSynWcCase(PKBReader *db, Synonym &first);

    std::unique_ptr<Result> handleNumSynCase(PKBReader *db, PQLNumber &first, Synonym &second);

    std::unique_ptr<Result> handleNumNumCase(PKBReader *db, PQLNumber &first, PQLNumber &second);

    std::unique_ptr<Result> handleNumWcCase(PKBReader *db, PQLNumber &first);

    std::unique_ptr<Result> handleWcSynCase(PKBReader *db, Synonym &second);

    std::unique_ptr<Result> handleWcNumCase(PKBReader *db, PQLNumber &second);

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
