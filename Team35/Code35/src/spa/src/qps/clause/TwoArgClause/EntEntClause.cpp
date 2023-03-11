#include "EntEntClause.h"

EntEntClause::EntEntClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
        NameNameRelationship rs) : TwoArgClause(std::move(first), std::move(second)), rs(rs) {}

std::unique_ptr<Result> EntEntClause::evaluate(PKBReader* db) {
    /* <SYNONYM | IDENT | _ >, <SYNONYM | IDENT | _ > */

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses/Modifies(f, v) -> <str, str>[]
        {
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses/Modifies(f, "x") -> str[]
        {
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses/Modifies(f, _) -> str[]
        {
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::SYNONYM):  // Uses/Mod("f", v) -> str[]
        {
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::IDENT):  // Uses/Modifies("f", "x") -> bool
        {
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::WILDCARD):  // Uses/Modifies("f", _) -> bool
        {
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::SYNONYM):  // Calls(_, f) -> str[]
        {
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::IDENT):  // Calls(_, "f") -> bool
        {
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // Calls(_, _) -> bool
        {
        }
        default:
            throw std::runtime_error("");
    }}

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

void Calls::validateArgs() {
    Synonym* synonym1 = dynamic_cast<Synonym*>(first.get());
    Synonym* synonym2 = dynamic_cast<Synonym*>(second.get());
    if ((synonym1 != nullptr && synonym1->de != Synonym::DesignEntity::PROCEDURE) ||
        (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::PROCEDURE)) {
        throw SemanticException();
    }
}
