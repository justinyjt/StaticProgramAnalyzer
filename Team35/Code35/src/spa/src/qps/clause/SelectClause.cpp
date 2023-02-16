#include "SelectClause.h"

SelectClause::SelectClause(Synonym syn) : syn(syn) {}

Result* SelectClause::evaluate(PKBReader* db) {
    // db->getEntities(Entity::Variable);
    // throw std::runtime_error();
    return nullptr;
}

bool SelectClause::operator==(const Clause &rhs) const {
    const SelectClause* pRhs = dynamic_cast<const SelectClause*>(&rhs);
    if (pRhs != nullptr) {
        return syn == pRhs->syn;
    }
    return false;
}
