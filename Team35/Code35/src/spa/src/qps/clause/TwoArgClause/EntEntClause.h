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
