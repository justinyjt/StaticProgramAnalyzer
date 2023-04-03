#include <cassert>
#include <vector>
#include <string>

#include "SelectClause.h"
#include "MultipleSynonymSelectClause.h"
#include "qps/pql/AttrRef.h"
#include "qps/query_parser/helper.h"
#include "qps/result/SelectResult.h"

MultipleSynonymSelectClause::MultipleSynonymSelectClause(std::vector<std::unique_ptr<Synonym>> selectedSynonyms)
        : SelectClause(SelectType::Synonym),
          selectedSynonyms_(std::move(selectedSynonyms)) {
    for (const auto &synonym : selectedSynonyms_) {
        this->addHeader(synonym->ident);
    }
}

STMT_SET MultipleSynonymSelectClause::getStmtSet(PKBReader *db, Synonym synonym) {
    switch (synonym.de) {
        case Synonym::DesignEntity::STMT:
            return db->getStatements(StmtType::None);
        case Synonym::DesignEntity::READ:
            return db->getStatements(StmtType::Read);
        case Synonym::DesignEntity::PRINT:
            return db->getStatements(StmtType::Print);
        case Synonym::DesignEntity::CALL:
            return db->getStatements(StmtType::Call);
        case Synonym::DesignEntity::WHILE:
            return db->getStatements(StmtType::While);
        case Synonym::DesignEntity::IF:
            return db->getStatements(StmtType::If);
        case Synonym::DesignEntity::ASSIGN:
            return db->getStatements(StmtType::Assign);
        default:
            throw std::runtime_error("Invalid synonym type");
    }
}

ENT_SET MultipleSynonymSelectClause::getEntSet(PKBReader *db, Synonym synonym) {
    switch (synonym.de) {
        case Synonym::DesignEntity::PROCEDURE:
            return db->getEntities(Entity::Procedure);
        case Synonym::DesignEntity::VARIABLE:
            return db->getEntities(Entity::Variable);
        case Synonym::DesignEntity::CONSTANT:
            return db->getEntities(Entity::Constant);
        default:
            throw std::runtime_error("Invalid synonym type");
    }
}

STMT_ENT_SET MultipleSynonymSelectClause::getStmtEntSet(PKBReader *db, Synonym synonym) {
    STMT_ENT_SET ses;

    switch (synonym.de) {
        case Synonym::DesignEntity::READ:
            return db->getAllRelationships(StmtNameRelationship::ReadStmtVar);
        case Synonym::DesignEntity::PRINT:
            return db->getAllRelationships(StmtNameRelationship::PrintStmtVar);
        case Synonym::DesignEntity::CALL:
            return db->getAllRelationships(StmtNameRelationship::CallsProcedure);
        default:
            throw std::runtime_error("Invalid synonym type");
    }
}

bool MultipleSynonymSelectClause::isEntAttrRef(Synonym &syn) {
    auto attrRef = dynamic_cast<AttrRef *>(&syn);
    return attrRef != nullptr && (attrRef->ref == PROCNAME_KEYWORD || attrRef->ref == VARNAME_KEYWORD);
}

// loop through synonyms and get tables
// E.g.
// a, v, x
// 1, 3, 1
// 3, 4, 2
// 3, 6
std::unique_ptr<Result> MultipleSynonymSelectClause::evaluate(PKBReader *db) {
    std::vector<std::string> idents;
    std::vector<TableResult> selectTables;

    for (auto &selectedSynonym : selectedSynonyms_) {
        // add ident and columns(tableResult)
        if (selectedSynonym->de == Synonym::DesignEntity::PROCEDURE ||
            selectedSynonym->de == Synonym::DesignEntity::VARIABLE ||
            selectedSynonym->de == Synonym::DesignEntity::CONSTANT) {
            ENT_SET es = getEntSet(db, *selectedSynonym);
            idents.emplace_back(selectedSynonym->ident);
            selectTables.emplace_back(selectedSynonym->ident, es);
        } else if (isEntAttrRef(*selectedSynonym)) {
            auto attrRef = dynamic_cast<AttrRef &>(*selectedSynonym);
            idents.emplace_back(selectedSynonym->ident + PERIOD + attrRef.ref);
            STMT_ENT_SET ses = getStmtEntSet(db, *selectedSynonym);
            selectTables.emplace_back(selectedSynonym->ident, selectedSynonym->ident + PERIOD + attrRef.ref, ses);
        } else {
            STMT_SET ss = getStmtSet(db, *selectedSynonym);
            idents.emplace_back(selectedSynonym->ident);
            selectTables.emplace_back(selectedSynonym->ident, ss);
        }
    }
    return std::make_unique<SelectResult>(idents, selectTables);;
}

bool MultipleSynonymSelectClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const MultipleSynonymSelectClause *>(&rhs);
    return pRhs != nullptr &&
           std::equal(selectedSynonyms_.begin(), selectedSynonyms_.end(), pRhs->selectedSynonyms_.begin(),
                      [](const std::unique_ptr<Synonym> &lhs, const std::unique_ptr<Synonym> &rhs) {
                          return *lhs == *rhs;
                      });
}
