#include "TwoArgClause.h"

#include <cassert>

TwoArgClause::TwoArgClause(std::unique_ptr<PQLToken> first,
                           std::unique_ptr<PQLToken> second) : first_(std::move(first)), second_(std::move(second)) {
    if (first_->tag == PQLToken::Tag::SYNONYM)
        this->addHeader(dynamic_cast<Synonym &>(*first_).ident);
    if (second_->tag == PQLToken::Tag::SYNONYM)
        this->addHeader(dynamic_cast<Synonym &>(*second_).ident);
}

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
    return *first_ == *(rhs.first_) && *second_ == *(rhs.second_);
}

int TwoArgClause::getPairEnum() const {
    return 10 * static_cast<int>(first_->tag) + static_cast<int>(second_->tag);
}
