#pragma once
#include <memory>
#include <utility>

#include "TwoArgClause.h"

class EntEntClause : public TwoArgClause {
 public:
    const NameNameRelationship rs;
    EntEntClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, NameNameRelationship);
    std::unique_ptr<Result> evaluate(PKBReader *) override;

 protected:
    static std::unique_ptr<Result> handleSynSyn(
        PKBReader *db, const std::pair<ENT_NAME, ENT_NAME> &argPair);

    static std::unique_ptr<Result> handleSynIdent(
        PKBReader *db, const std::pair<ENT_NAME, ENT_NAME> &argPair);

    static std::unique_ptr<Result> handleSynWc(PKBReader *db, const ENT_NAME &synName);

    static std::unique_ptr<Result> handleIdentSyn(
        PKBReader *db, const std::pair<ENT_NAME, ENT_NAME> &argPair);

    static std::unique_ptr<Result> handleIdentIdent(
        PKBReader *db, const std::pair<ENT_NAME, ENT_NAME> &argPair);

    static std::unique_ptr<Result> handleIdentWc(PKBReader *db, const ENT_NAME &entName);

    static std::unique_ptr<Result> handleWcSyn(PKBReader *db, const ENT_NAME &synName);

    static std::unique_ptr<Result> handleWcIdent(PKBReader *db, const ENT_NAME &entName);

    static std::unique_ptr<Result> handleWcWc(PKBReader *db);

    bool operator==(const Clause &rhs) const override;
};

class UsesP : public EntEntClause {
 public:
    UsesP(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    void validateArgs() override;
};

class ModifiesP : public EntEntClause {
 public:
    ModifiesP(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    void validateArgs() override;
};

class Calls : public EntEntClause {
 public:
    Calls(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, bool isTransitive);
    void validateArgs() override;
};
