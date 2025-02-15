#include "PKB.h"

#include <utility>

#include "db/exception/TableException.h"

const EntityTable<ENT_NAME> &PKB::getEntityTable(Entity entityType) const {
    switch (entityType) {
        case Entity::Variable:
            return variableTable_;
        case Entity::Constant:
            return constantTable_;
        case Entity::Procedure:
            return procedureTable_;
        default:
            throw TableException();
    }
}

const EntityTable<STMT_NUM> &PKB::getStatementTable(StmtType stmtType) const {
    switch (stmtType) {
        case StmtType::Assign:
            return assignStatementTable_;
        case StmtType::Print:
            return printStatementTable_;
        case StmtType::Read:
            return readStatementTable_;
        case StmtType::If:
            return ifStatementTable_;
        case StmtType::While:
            return whileStatementTable_;
        case StmtType::Call:
            return callStatementTable_;
        case StmtType::None:
            return statementTable_;
        default:
            throw TableException();
    }
}

EntityTable<ENT_NAME> &PKB::getEntityTable(Entity entityType) {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<EntityTable<ENT_NAME> &>(pkbPtr->getEntityTable(entityType));
}

EntityTable<STMT_NUM> &PKB::getStatementTable(StmtType stmtType) {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<EntityTable<STMT_NUM> &>(pkbPtr->getStatementTable(stmtType));
}

const RelationshipTable<STMT_NUM, ENT_NAME> &PKB::getStmtNameRelationshipTable(StmtNameRelationship tableType) const {
    switch (tableType) {
        case StmtNameRelationship::Modifies:
            return modifiesStmtNameTable_;
        case StmtNameRelationship::Uses:
            return usesStmtNameTable_;
        case StmtNameRelationship::IfCondVarUses:
            return ifCondUsesVarTable_;
        case StmtNameRelationship::WhileCondVarUses:
            return whileCondUsesVarTable_;
        case StmtNameRelationship::ContainerProcedure:
            return containerProcedureTable_;
        case StmtNameRelationship::CallsProcedure:
            return callsStmtProcedureTable_;
        case StmtNameRelationship::ReadStmtVar:
            return readStmtVarTable_;
        case StmtNameRelationship::PrintStmtVar:
            return printStmtVarTable_;
        default:
            throw TableException();
    }
}

RelationshipTable<STMT_NUM, ENT_NAME> &PKB::getStmtNameRelationshipTable(StmtNameRelationship tableType) {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<STMT_NUM, ENT_NAME> &>(pkbPtr->getStmtNameRelationshipTable(tableType));
}

const RelationshipTable<ENT_NAME, ENT_NAME> &PKB::getNameNameRelationshipTable(NameNameRelationship tableType) const {
    switch (tableType) {
        case NameNameRelationship::Modifies:
            return modifiesNameNameTable_;
        case NameNameRelationship::Uses:
            return usesNameNameTable_;
        case NameNameRelationship::Calls:
            return callsNameNameTable_;
        case NameNameRelationship::CallsStar:
            return callsStarNameNameTable_;
        default:
            throw TableException();
    }
}

RelationshipTable<STMT_NUM, STMT_NUM> &PKB::getStmtStmtRelationshipTable(StmtStmtRelationship tableType) {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<STMT_NUM, STMT_NUM> &>(pkbPtr->getStmtStmtRelationshipTable(tableType));
}

const RelationshipTable<STMT_NUM, STMT_NUM> &PKB::getStmtStmtRelationshipTable(StmtStmtRelationship tableType) const {
    switch (tableType) {
        case StmtStmtRelationship::Parent:
            return parentTable_;
        case StmtStmtRelationship::ParentStar:
            return parentStarTable_;
        case StmtStmtRelationship::Follows:
            return followsTable_;
        case StmtStmtRelationship::FollowsStar:
            return followsStarTable_;
        default:
            throw TableException();
    }
}

RelationshipTable<ENT_NAME, ENT_NAME> &PKB::getNameNameRelationshipTable(NameNameRelationship tableType) {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<ENT_NAME, ENT_NAME> &>(pkbPtr->getNameNameRelationshipTable(tableType));
}

void PKB::addEntityToTable(Entity entityType, ENT_NAME entity) {
    EntityTable<ENT_NAME> &table = this->getEntityTable(entityType);
    table.addEntity(entity);
}

void PKB::addStatementToTable(StmtType stmtType, STMT_NUM stmtNum) {
    EntityTable<STMT_NUM> &table = this->getStatementTable(stmtType);
    table.addEntity(stmtNum);
}

void PKB::addRelationshipToTable(StmtNameRelationship tableType, const STMT_ENT &stmtEnt) {
    RelationshipTable<STMT_NUM, ENT_NAME> &table = this->getStmtNameRelationshipTable(tableType);
    table.insertPair(stmtEnt.first, stmtEnt.second);
}

void PKB::addRelationshipToTable(NameNameRelationship tableType, const ENT_ENT &entEnt) {
    RelationshipTable<ENT_NAME, ENT_NAME> &table = this->getNameNameRelationshipTable(tableType);
    table.insertPair(entEnt.first, entEnt.second);
}

void PKB::addRelationshipToTable(StmtStmtRelationship tableType, STMT_STMT stmtStmt) {
    RelationshipTable<STMT_NUM, STMT_NUM> &table = this->getStmtStmtRelationshipTable(tableType);
    table.insertPair(stmtStmt.first, stmtStmt.second);
}

void PKB::addPattern(STMT_NUM stmtNum, ASSIGN_PAT pattern) {
    patternTable_.addPattern(stmtNum, std::move(pattern));
}

void PKB::addCallGraph(CallGraph &&callGraph) {
    this->callGraph_ = std::move(callGraph);
}

void PKB::addCFGraphs(std::vector<CFG::CFGraph> &&CFGraphs) {
    this->cfgManager_.setGraphs(std::move(CFGraphs));
}

ENT_SET PKB::getEntByStmtKey(StmtNameRelationship tableType, STMT_NUM stmt) const {
    return getStmtNameRelationshipTable(tableType).getValuesByKey(stmt);
}

STMT_SET PKB::getStmtByEntVal(StmtNameRelationship tableType, const ENT_NAME &name) const {
    return getStmtNameRelationshipTable(tableType).getKeysByValue(name);
}

STMT_ENT_SET PKB::getStmtEntSet(StmtNameRelationship tableType) const {
    return getStmtNameRelationshipTable(tableType).getKeyValuePairs();
}

STMT_SET PKB::getStmtByRs(StmtNameRelationship tableType) const {
    return getStmtNameRelationshipTable(tableType).getKeys();
}

bool PKB::isStmtEntPairExists(StmtNameRelationship tableType, STMT_NUM stmt, const ENT_NAME &name) const {
    return getStmtNameRelationshipTable(tableType).containsPair(stmt, name);
}

ENT_SET PKB::getEntByEntKey(NameNameRelationship tableType, const ENT_NAME &name) const {
    return getNameNameRelationshipTable(tableType).getValuesByKey(name);
}

ENT_SET PKB::getEntByEntVal(NameNameRelationship tableType, const ENT_NAME &name) const {
    return getNameNameRelationshipTable(tableType).getKeysByValue(name);
}

ENT_ENT_SET PKB::getEntEntSet(NameNameRelationship tableType) const {
    return getNameNameRelationshipTable(tableType).getKeyValuePairs();
}

ENT_SET PKB::getKeyNameByRs(NameNameRelationship tableType) const {
    return getNameNameRelationshipTable(tableType).getKeys();
}

ENT_SET PKB::getValNameByRs(NameNameRelationship tableType) const {
    return getNameNameRelationshipTable(tableType).getValues();
}

bool PKB::isEntEntPairExists(NameNameRelationship tableType, const ENT_NAME &key, const ENT_NAME &val) const {
    return getNameNameRelationshipTable(tableType).containsPair(key, val);
}

STMT_SET PKB::getStmtByStmtKey(StmtStmtRelationship tableType, STMT_NUM stmt) const {
    switch (tableType) {
        case StmtStmtRelationship::Next:
            return cfgManager_.getConnectedStmts(stmt, ArgType::Key, UsageType::Direct);
        case StmtStmtRelationship::NextStar:
            return cfgManager_.getConnectedStmts(stmt, ArgType::Key, UsageType::Transitive);
        default:
            return getStmtStmtRelationshipTable(tableType).getValuesByKey(stmt);
    }
}

STMT_SET PKB::getStmtByStmtVal(StmtStmtRelationship tableType, STMT_NUM stmt) const {
    switch (tableType) {
        case StmtStmtRelationship::Next:
            return cfgManager_.getConnectedStmts(stmt, ArgType::Value, UsageType::Direct);
        case StmtStmtRelationship::NextStar:
            return cfgManager_.getConnectedStmts(stmt, ArgType::Value, UsageType::Transitive);
        default:
            return getStmtStmtRelationshipTable(tableType).getKeysByValue(stmt);
    }
}

STMT_STMT_SET PKB::getStmtStmtSet(StmtStmtRelationship tableType) {
    switch (tableType) {
        case StmtStmtRelationship::Next:
            return cfgManager_.getValidNextPairs(UsageType::Direct);
        case StmtStmtRelationship::NextStar:
            return cfgManager_.getValidNextPairs(UsageType::Transitive);
        default:
            return getStmtStmtRelationshipTable(tableType).getKeyValuePairs();
    }
}

STMT_SET PKB::getKeyStmtByRs(StmtStmtRelationship tableType) const {
    switch (tableType) {
        case StmtStmtRelationship::Next:
        case StmtStmtRelationship::NextStar:
            return cfgManager_.getValidPredecessors();
        default:
            return getStmtStmtRelationshipTable(tableType).getKeys();
    }
}

STMT_SET PKB::getValStmtByRs(StmtStmtRelationship tableType) const {
    switch (tableType) {
        case StmtStmtRelationship::Next:
        case StmtStmtRelationship::NextStar:
            return cfgManager_.getValidSuccessors();
        default:
            return getStmtStmtRelationshipTable(tableType).getValues();
    }
}

bool PKB::isStmtStmtPairExists(StmtStmtRelationship tableType, STMT_NUM key, STMT_NUM val) const {
    switch (tableType) {
        case StmtStmtRelationship::Next:
            return cfgManager_.isNext(key, val, UsageType::Direct);
        case StmtStmtRelationship::NextStar:
            return cfgManager_.isNext(key, val, UsageType::Transitive);
        default:
            return getStmtStmtRelationshipTable(tableType).containsPair(key, val);
    }
}

bool PKB::isEntityTypeExists(StmtType tableType, STMT_NUM key) const {
    return this->getStatementTable(tableType).hasEntity(key);
}

bool PKB::isRelationshipExists(StmtStmtRelationship tableType) {
    switch (tableType) {
        case StmtStmtRelationship::NextStar:
        case StmtStmtRelationship::Next:
            return cfgManager_.isNextExists();
        default:
            return !this->getStmtStmtSet(tableType).empty();
    }
}

STMT_SET PKB::getStmtWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return this->patternTable_.getExactPatternMatch(pattern);
}

STMT_SET PKB::getStmtWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return this->patternTable_.getPartialPatternMatch(pattern);
}

STMT_ENT_SET PKB::getStmtVarWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return this->patternTable_.getExactPatternMatchWithLeft(pattern);
}

STMT_ENT_SET PKB::getStmtVarWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return this->patternTable_.getPartialPatternMatchWithLeft(pattern);
}

STMT_SET PKB::getStmtIntersectWithExactPatternMatch(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT &right) const {
    return this->patternTable_.getExactPatternIntersect(left, right);
}

STMT_SET PKB::getStmtIntersectWithPartialPatternMatch(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT &right) const {
    return this->patternTable_.getPartialPatternIntersect(left, right);
}
