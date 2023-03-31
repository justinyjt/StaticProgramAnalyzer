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

std::unique_ptr<Result> MultipleSynonymSelectClause::evaluate(PKBReader *db) {
    // loop through synonyms and get tables
    // E.g.
    // a, v, x
    // 1, 3, 1
    // 3, 4, 2
    // 3, 6
    std::vector<std::string> idents;
    std::vector<TableResult> selectTables;

    for (auto &selectedSynonym : selectedSynonyms_) {
        std::unique_ptr<Synonym> synonym = std::move(selectedSynonym);
        STMT_ENT_SET ses;
        STMT_SET ss;
        ENT_SET es;
        switch (synonym->de) {
            case Synonym::DesignEntity::PROCEDURE:
                es = db->getEntities(Entity::Procedure);
                break;
            case Synonym::DesignEntity::VARIABLE:
                es = db->getEntities(Entity::Variable);
                break;
            case Synonym::DesignEntity::CONSTANT:
                es = db->getEntities(Entity::Constant);
                break;
            case Synonym::DesignEntity::STMT:
                ss = db->getStatements(StmtType::None);
                break;
            case Synonym::DesignEntity::READ: {
                // check if varName - get stmt-ent-set, else get stmt-set
                auto attrRef = dynamic_cast<AttrRef *>(synonym.get());
                if (attrRef != nullptr && attrRef->ref == VARNAME_KEYWORD) {
                    ses = db->getAllRelationships(StmtNameRelationship::ReadStmtVar);
                } else {
                    ss = db->getStatements(StmtType::Read);
                }
                break;
            }
            case Synonym::DesignEntity::PRINT: {
                // check if varName - get stmt-ent-set, else get stmt-set
                auto attrRef = dynamic_cast<AttrRef *>(synonym.get());
                if (attrRef != nullptr && attrRef->ref == VARNAME_KEYWORD) {
                    ses = db->getAllRelationships(StmtNameRelationship::PrintStmtVar);
                } else {
                    ss = db->getStatements(StmtType::Print);
                }
                break;
            }
            case Synonym::DesignEntity::CALL: {
                // check if varName - get stmt-ent-set, else get stmt-set
                auto attrRef = dynamic_cast<AttrRef *>(synonym.get());
                if (attrRef != nullptr && attrRef->ref == PROCNAME_KEYWORD) {
                    ses = db->getAllRelationships(StmtNameRelationship::CallsProcedure);
                } else {
                    ss = db->getStatements(StmtType::Call);
                }
                break;
            }
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
                assert(false);
        }

        // add ident and columns(tableResult)
        if (synonym->de == Synonym::DesignEntity::PROCEDURE || synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT) {
            idents.emplace_back(synonym->ident);
            selectTables.emplace_back(synonym->ident, es);
        } else {
            auto attrRef = dynamic_cast<AttrRef *>(synonym.get());
            if (attrRef != nullptr && (attrRef->ref == VARNAME_KEYWORD || attrRef->ref == PROCNAME_KEYWORD)) {
                idents.emplace_back(synonym->ident + "." + attrRef->ref);
                selectTables.emplace_back(synonym->ident, synonym->ident + "." + attrRef->ref, ses);
            } else {
                idents.emplace_back(synonym->ident);
                selectTables.emplace_back(synonym->ident, ss);
            }
        }
    }
    std::unique_ptr<SelectResult> selectResult = std::make_unique<SelectResult>(idents, selectTables);
    return std::move(selectResult);
}

bool MultipleSynonymSelectClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const MultipleSynonymSelectClause *>(&rhs);
    return pRhs != nullptr &&
        std::equal(selectedSynonyms_.begin(), selectedSynonyms_.end(), pRhs->selectedSynonyms_.begin(),
                   [](const std::unique_ptr<Synonym> &lhs, const std::unique_ptr<Synonym> &rhs) {
                     return *lhs == *rhs;
                   });
}
