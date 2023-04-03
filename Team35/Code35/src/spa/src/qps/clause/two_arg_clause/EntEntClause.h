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
        PKBReader *db, std::pair<ENT_NAME, ENT_NAME> argPair, NameNameRelationship rs);

    static std::unique_ptr<Result> handleSynIdent(
        PKBReader *db, std::pair<ENT_NAME, ENT_NAME> argPair, NameNameRelationship rs);

    static std::unique_ptr<Result> handleSynWc(PKBReader *db, ENT_NAME synName, NameNameRelationship rs);

    static std::unique_ptr<Result> handleIdentSyn(
        PKBReader *db, std::pair<ENT_NAME, ENT_NAME> argPair, NameNameRelationship rs);

    static std::unique_ptr<Result> handleIdentIdent(
        PKBReader *db, std::pair<ENT_NAME, ENT_NAME> argPair, NameNameRelationship rs);

    static std::unique_ptr<Result> handleIdentWc(PKBReader *db, ENT_NAME entName, NameNameRelationship rs);

    static std::unique_ptr<Result> handleWcSyn(PKBReader *db, ENT_NAME synName, NameNameRelationship rs);

    static std::unique_ptr<Result> handleWcIdent(PKBReader *db, ENT_NAME entName, NameNameRelationship rs);

    static std::unique_ptr<Result> handleWcWc(PKBReader *db, NameNameRelationship rs);

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
