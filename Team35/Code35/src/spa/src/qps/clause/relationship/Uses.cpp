#include "Uses.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/Synonym.h"
#include "qps/query_exceptions/SemanticException.h"

UsesS::UsesS(Tok* first, Tok* second) : Relationship(first, second) {
    validateArgs(first, second);
}

Result* UsesS::evaluate(PKBReader *db) {
    throw std::runtime_error("");
}

void UsesS::validateArgs(Tok* first, Tok* second) {
    const Wildcard* wildcard = dynamic_cast<const Wildcard*>(first);
    const Synonym* synonym1 = dynamic_cast<const Synonym*>(first);
    const Synonym* synonym2 = dynamic_cast<const Synonym*>(second);
    if (synonym1 != NULL && (synonym1->de == Synonym::DesignEntity::READ) ||
        wildcard != NULL ||
        (synonym2 != NULL && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}

bool UsesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const UsesS*>(&rhs) != NULL) && Clause::equal(rhs);
}
