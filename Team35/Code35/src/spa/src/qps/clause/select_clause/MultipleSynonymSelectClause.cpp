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

std::pair<STMT_SET, std::pair<ENT_SET, STMT_ENT_SET>> MultipleSynonymSelectClause::getSynonymSet(
    PKBReader *db, Synonym synonym) {
    STMT_SET ss;
    ENT_SET es;
    STMT_ENT_SET ses;

    switch (synonym.de) {
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
            if (isEntAttrRef(synonym)) {
                ses = db->getAllRelationships(StmtNameRelationship::ReadStmtVar);
            } else {
                ss = db->getStatements(StmtType::Read);
            }
            break;
        }
        case Synonym::DesignEntity::PRINT: {
            // check if varName - get stmt-ent-set, else get stmt-set
            if (isEntAttrRef(synonym)) {
                ses = db->getAllRelationships(StmtNameRelationship::PrintStmtVar);
            } else {
                ss = db->getStatements(StmtType::Print);
            }
            break;
        }
        case Synonym::DesignEntity::CALL: {
            // check if varName - get stmt-ent-set, else get stmt-set
            if (isEntAttrRef(synonym)) {
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
            throw std::runtime_error("Invalid synonym type");
    }

    return std::make_pair(ss, std::make_pair(es, ses));
}

bool MultipleSynonymSelectClause::isEntAttrRef(Synonym syn) {
    auto attrRef = dynamic_cast<AttrRef *>(&syn);
    return attrRef != nullptr && (attrRef->ref == PROCNAME_KEYWORD ||  attrRef->ref == VARNAME_KEYWORD);
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
        std::pair<STMT_SET, std::pair<ENT_SET, STMT_ENT_SET>> syn_sets = getSynonymSet(db, *selectedSynonym);
        STMT_SET ss = syn_sets.first;
        ENT_SET es = syn_sets.second.first;
        STMT_ENT_SET ses = syn_sets.second.second;

        // add ident and columns(tableResult)
        if (selectedSynonym->de == Synonym::DesignEntity::PROCEDURE ||
            selectedSynonym->de == Synonym::DesignEntity::VARIABLE ||
            selectedSynonym->de == Synonym::DesignEntity::CONSTANT) {
            idents.emplace_back(selectedSynonym->ident);
            selectTables.emplace_back(selectedSynonym->ident, es);
        } else if (isEntAttrRef(*selectedSynonym)) {
            auto attrRef = dynamic_cast<AttrRef &>(*selectedSynonym);
            idents.emplace_back(selectedSynonym->ident + PERIOD + attrRef.ref);
            selectTables.emplace_back(selectedSynonym->ident, selectedSynonym->ident + PERIOD + attrRef.ref, ses);
        } else {
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
