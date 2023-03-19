#include "TwoArgClause.h"

#include <cassert>

TwoArgClause::TwoArgClause(std::unique_ptr<PQLToken> first,
                           std::unique_ptr<PQLToken> second) : first(std::move(first)), second(std::move(second)) {}

StmtType TwoArgClause::getStmtType(Synonym::DesignEntity type) {
    switch (type) {
        case Synonym::DesignEntity::READ:
            return StmtType::Read;
        case Synonym::DesignEntity::PRINT:
            return StmtType::Print;
        case Synonym::DesignEntity::WHILE:
            return StmtType::While;
        case Synonym::DesignEntity::IF:
            return StmtType::If;
        case Synonym::DesignEntity::ASSIGN:
            return StmtType::Assign;
        case Synonym::DesignEntity::CALL:
            return StmtType::Call;
        case Synonym::DesignEntity::STMT:
            return StmtType::None;
        default:
            assert(false);
    }
}

bool TwoArgClause::equal(const TwoArgClause &rhs) const {
    return *first == *(rhs.first) && *second == *(rhs.second);
}

int TwoArgClause::getPairEnum() const {
    return 10 * static_cast<int>(first->tag) + static_cast<int>(second->tag);
}
