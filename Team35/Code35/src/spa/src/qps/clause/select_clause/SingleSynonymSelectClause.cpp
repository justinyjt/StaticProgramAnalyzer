#include <cassert>

#include "SelectClause.h"
#include "SingleSynonymSelectClause.h"
#include "qps/pql/AttrRef.h"
#include "qps/query_parser/helper.h"
#include "qps/result/SelectResult.h"

SingleSynonymSelectClause::SingleSynonymSelectClause(std::unique_ptr<Synonym> syn) : syn(std::move(syn)) {}

std::unique_ptr<Result> SingleSynonymSelectClause::evaluate(PKBReader *db) {
    STMT_ENT_SET ses;
    STMT_SET ss;
    ENT_SET es;
    std::unique_ptr<Result> result;

    switch (syn->de) {
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
            auto attrRef = dynamic_cast<AttrRef*>(syn.get());
            if (attrRef != nullptr && attrRef->ref == VARNAME_KEYWORD) {
                ses = db->getAllRelationships(StmtNameRelationship::ReadStmtVar);
            } else {
                ss = db->getStatements(StmtType::Read);
            }
            break;
        }
        case Synonym::DesignEntity::PRINT: {
            // check if varName - get stmt-ent-set, else get stmt-set
            auto attrRef = dynamic_cast<AttrRef *>(syn.get());
            if (attrRef != nullptr && attrRef->ref == VARNAME_KEYWORD) {
                ses = db->getAllRelationships(StmtNameRelationship::PrintStmtVar);
            } else {
                ss = db->getStatements(StmtType::Print);
            }
            break;
        }
        case Synonym::DesignEntity::CALL: {
            // check if varName - get stmt-ent-set, else get stmt-set
            auto attrRef = dynamic_cast<AttrRef*>(syn.get());
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

    if (syn->de == Synonym::DesignEntity::PROCEDURE || syn->de == Synonym::DesignEntity::VARIABLE ||
        syn->de == Synonym::DesignEntity::CONSTANT) {
        selectedIdent = syn->ident;
        result = std::make_unique<SelectResult>(syn->ident, es, syn->ident);
    } else {
        auto attrRef = dynamic_cast<AttrRef *>(syn.get());
        if (attrRef != nullptr && (attrRef->ref == VARNAME_KEYWORD || attrRef->ref == PROCNAME_KEYWORD)) {
            selectedIdent = syn->ident + attrRef->ref;
            result = std::make_unique<SelectResult>(syn->ident, syn->ident + attrRef->ref,ses, syn->ident + attrRef->ref);
        } else {
            selectedIdent = syn->ident;
            result = std::make_unique<SelectResult>(syn->ident, ss, syn->ident);
        }
    }

    return std::move(result);
}

bool SingleSynonymSelectClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const SingleSynonymSelectClause *>(&rhs);
    return pRhs != nullptr && *syn == *(pRhs->syn);
}
