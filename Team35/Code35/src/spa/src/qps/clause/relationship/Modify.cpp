#include "Modify.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Wildcard.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/pql/Synonym.h"

ModifiesS::ModifiesS(Tok* first, Tok* second) : Relationship(first, second) {
    validateArgs(first, second);
}

void ModifiesS::validateArgs(Tok* first, Tok* second) {
    const Wildcard* wildcard = dynamic_cast<const Wildcard*>(first);
    const Synonym* synonym1 = dynamic_cast<const Synonym*>(first);
    const Synonym* synonym2 = dynamic_cast<const Synonym*>(second);
    if (synonym1 != NULL && (synonym1->de == Synonym::DesignEntity::PRINT) ||
        wildcard != NULL ||
        (synonym2 != NULL && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}

// select v such that modifies(1, v)

Result* ModifiesS::evaluate(PKBReader *db) {
    if (first->tag == Tok::Tag::STMT_NUM && second->tag == Tok::Tag::SYNONYM) {
        ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies,
                                          (dynamic_cast<const StatementNumber*>(first))->n);

        StrResult *result = new StrResult(set);
        return dynamic_cast<Result *>(result);
    }

    throw std::runtime_error("");
}


bool ModifiesS::operator==(const Clause& rhs) const {
    return (dynamic_cast<const ModifiesS*>(&rhs) != NULL) && Clause::equal(rhs);
}
