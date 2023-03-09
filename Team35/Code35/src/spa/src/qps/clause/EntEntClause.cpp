#include "EntEntClause.h"

EntEntClause::EntEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                           NameNameRelationship rs) : TwoArgClause(std::move(first), std::move(second)), rs(rs) {}

std::unique_ptr<Result> EntEntClause::evaluate(PKBReader* db) {
    /* <SYNONYM | IDENT | _ >, <SYNONYM | IDENT | _ > */

    throw std::runtime_error("");
}

bool EntEntClause::operator==(const Clause &rhs) const {
    const auto* pRhs = dynamic_cast<const EntEntClause*>(&rhs);
    return pRhs != nullptr && rs == pRhs->rs && TwoArgClause::equal(*pRhs);
}

UsesP::UsesP(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
        EntEntClause(std::move(first), std::move(second), NameNameRelationship::Uses) {
    validateArgs();
}

void UsesP::validateArgs() {
    if (dynamic_cast<Wildcard*>(first.get()) != nullptr) throw SemanticException();
}


ModifiesP::ModifiesP(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second) :
        EntEntClause(std::move(first), std::move(second), NameNameRelationship::Modifies) {
    validateArgs();
}

void ModifiesP::validateArgs() {
    if (dynamic_cast<Wildcard*>(first.get()) != nullptr) throw SemanticException();
}


Calls::Calls(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second, bool isTransitive) :
        EntEntClause(std::move(first), std::move(second),
                     isTransitive ? NameNameRelationship::CallsStar : NameNameRelationship::Calls) {
    validateArgs();
}

void Calls::validateArgs() {}