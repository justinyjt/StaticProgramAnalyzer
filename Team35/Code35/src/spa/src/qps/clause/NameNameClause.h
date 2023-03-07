#pragma once
#include <memory>
#include <utility>

#include "TwoArgClause.h"

class NameNameClause : public TwoArgClause {
 public:
    const NameNameRelationship rs;
    NameNameClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>, NameNameRelationship);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
 protected:
    bool operator==(const Clause &rhs) const override;
};

class UsesP : public NameNameClause {
 public:
    UsesP(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    void validateArgs() override;
};

class ModifiesP : public NameNameClause {
 public:
    ModifiesP(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    void validateArgs() override;
};
