#include <cassert>
#include <vector>

#include "SelectClause.h"
#include "MultipleSynonymSelectClause.h"
#include "qps/pql/AttrRef.h"
#include "qps/query_parser/helper.h"
#include "qps/result/SelectResult.h"

MultipleSynonymSelectClause::MultipleSynonymSelectClause(std::vector<std::unique_ptr<Synonym>>
                                                         selectedSynonyms) : selectedSynonyms(std::move(selectedSynonyms)) {}

std::unique_ptr<Result> MultipleSynonymSelectClause::evaluate(PKBReader *db) {
    // loop through synonyms and get tables
    // E.g.
    // a, v, x
    // 1, 3, 1
    // 3, 4, 2
    // 3, 6
    std::vector<std::string> idents;
    std::vector<std::vector<std::string>> columns;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> synToAttrRefMap;

    for (int i = 0; i < selectedSynonyms.size(); i++) {
        std::unique_ptr<Synonym> synonym = std::move(selectedSynonyms.at(i));
        idents.emplace_back(selectedSynonyms.at(i)->ident);
        std::unordered_map<std::string, std::string> synMap;
        std::vector<std::string> column;
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
                auto attrRef = dynamic_cast<AttrRef*>(synonym.get());
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
                auto attrRef = dynamic_cast<AttrRef*>(synonym.get());
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

        // create map from es, ss or ses
        if (synonym->de == Synonym::DesignEntity::PROCEDURE || synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT) {
            for (const auto& elem : es) {
                synMap.emplace(elem, elem);
                column.emplace_back(elem);
            }
            std::vector<std::string> column(es.begin(), es.end());
            columns.emplace_back(column);
        } else {
            auto attrRef = dynamic_cast<AttrRef *>(synonym.get());
            if (attrRef != nullptr && (attrRef->ref == VARNAME_KEYWORD || attrRef->ref == PROCNAME_KEYWORD)) {
                for (const auto& elem : ses) {
                    synMap.insert({std::to_string(elem.first), elem.second});
                    column.emplace_back(std::to_string(elem.first));
                }
            } else {
                for (const auto& elem : ss) {
                    synMap.insert({std::to_string(elem), std::to_string(elem)});
                    column.emplace_back(std::to_string(elem));
                }
            }
        }
        synToAttrRefMap.insert({selectedSynonyms.at(i)->ident, synMap});
        columns.emplace_back(column);
        std::unique_ptr<SelectResult> selectClause = std::make_unique<SelectResult>(idents, columns, synToAttrRefMap);
        return std::move(selectClause);
    }
//    STMT_ENT_SET ses;
//    STMT_SET ss;
//    ENT_SET es;
//    std::unique_ptr<Result> result;
//
//    switch (syn->de) {
//        case Synonym::DesignEntity::PROCEDURE:
//            es = db->getEntities(Entity::Procedure);
//            break;
//        case Synonym::DesignEntity::VARIABLE:
//            es = db->getEntities(Entity::Variable);
//            break;
//        case Synonym::DesignEntity::CONSTANT:
//            es = db->getEntities(Entity::Constant);
//            break;
//        case Synonym::DesignEntity::STMT:
//            ss = db->getStatements(StmtType::None);
//            break;
//        case Synonym::DesignEntity::READ: {
//            // check if varName - get stmt-ent-set, else get stmt-set
//            auto attrRef = dynamic_cast<AttrRef*>(syn.get());
//            if (attrRef != nullptr && attrRef->ref == VARNAME_KEYWORD) {
//                ses = db->getAllRelationships(StmtNameRelationship::ReadStmtVar);
//            } else {
//                ss = db->getStatements(StmtType::Read);
//            }
//            break;
//        }
//        case Synonym::DesignEntity::PRINT: {
//            // check if varName - get stmt-ent-set, else get stmt-set
//            auto attrRef = dynamic_cast<AttrRef *>(syn.get());
//            if (attrRef != nullptr && attrRef->ref == VARNAME_KEYWORD) {
//                ses = db->getAllRelationships(StmtNameRelationship::PrintStmtVar);
//            } else {
//                ss = db->getStatements(StmtType::Print);
//            }
//            break;
//        }
//        case Synonym::DesignEntity::CALL: {
//            // check if varName - get stmt-ent-set, else get stmt-set
//            auto attrRef = dynamic_cast<AttrRef*>(syn.get());
//            if (attrRef != nullptr && attrRef->ref == PROCNAME_KEYWORD) {
//                ses = db->getAllRelationships(StmtNameRelationship::CallsProcedure);
//            } else {
//                ss = db->getStatements(StmtType::Call);
//            }
//            break;
//        }
//        case Synonym::DesignEntity::WHILE:
//            ss = db->getStatements(StmtType::While);
//            break;
//        case Synonym::DesignEntity::IF:
//            ss = db->getStatements(StmtType::If);
//            break;
//        case Synonym::DesignEntity::ASSIGN:
//            ss = db->getStatements(StmtType::Assign);
//            break;
//        default:
//            assert(false);
//    }
//
//    if (syn->de == Synonym::DesignEntity::PROCEDURE || syn->de == Synonym::DesignEntity::VARIABLE ||
//        syn->de == Synonym::DesignEntity::CONSTANT) {
//        selectedIdent = syn->ident;
//        result = std::make_unique<SelectResult>(syn->ident, es, syn->ident);
//    } else {
//        auto attrRef = dynamic_cast<AttrRef *>(syn.get());
//        if (attrRef != nullptr && (attrRef->ref == VARNAME_KEYWORD || attrRef->ref == PROCNAME_KEYWORD)) {
//            selectedIdent = syn->ident + attrRef->ref;
//            result = std::make_unique<SelectResult>(syn->ident, syn->ident + attrRef->ref,
//                                                    ses, syn->ident + attrRef->ref);
//        } else {
//            selectedIdent = syn->ident;
//            result = std::make_unique<SelectResult>(syn->ident, ss, syn->ident);
//        }
//    }
//
//    return std::move(result);
//
//    throw std::runtime_error("");
}

bool MultipleSynonymSelectClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const MultipleSynonymSelectClause *>(&rhs);
    return pRhs != nullptr && selectedSynonyms == pRhs->selectedSynonyms;
}
