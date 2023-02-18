#include "SelectClause.h"

SelectClause::SelectClause(Synonym syn) : syn(syn) {}

std::unique_ptr<Result> SelectClause::evaluate(PKBReader* db) {
    STMT_SET ss;
    ENT_SET es;
    std::unique_ptr<Result> result;

    switch (syn.de) {
        case Synonym::DesignEntity::VARIABLE:
            es = db->getEntities(Entity::Variable);
        case Synonym::DesignEntity::CONSTANT:
            es = db->getEntities(Entity::Constant);
        case Synonym::DesignEntity::STMT:
            ss = db->getStatements(StmtType::None);
            break;
        case Synonym::DesignEntity::READ:
            ss = db->getStatements(StmtType::Read);
            break;
        case Synonym::DesignEntity::PRINT:
            ss = db->getStatements(StmtType::Print);
            break;
        case Synonym::DesignEntity::CALL:
            ss = db->getStatements(StmtType::Call);
            break;
        case Synonym::DesignEntity::WHILE:
            ss = db->getStatements(StmtType::While);
            break;
        case Synonym::DesignEntity::IF:
            ss = db->getStatements(StmtType::If);
            break;
        case Synonym::DesignEntity::ASSIGN:
            ss = db->getStatements(StmtType::Assign);
            break;
        default:
            throw std::runtime_error("SelectClause Error");
    }

    if (syn.de == Synonym::DesignEntity::VARIABLE ||
            syn.de == Synonym::DesignEntity::CONSTANT) {
        result = std::make_unique<TableResult>(syn.ident, es);
    } else {
        result = std::make_unique<TableResult>(syn.ident, ss);
    }

    return result;
}

bool SelectClause::operator==(const Clause &rhs) const {
    const SelectClause* pRhs = dynamic_cast<const SelectClause*>(&rhs);
    if (pRhs != nullptr) {
        return syn == pRhs->syn;
    }
    return false;
}
