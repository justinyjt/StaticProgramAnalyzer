#include "PKB.h"

#include <cassert>

const EntityTable<ENT_NAME> &PKB::getEntityTable(Entity entityType) const {
    switch (entityType) {
        case Entity::Variable:
            return variableTable;
        case Entity::Constant:
            return constantTable;
        case Entity::Procedure:
            return procedureTable;
        default:
            assert(false);
    }
}

const EntityTable<STMT_NUM> &PKB::getStatementTable(StmtType stmtType) const {
    switch (stmtType) {
        case StmtType::Assign:
            return assignStatementTable;
        case StmtType::Print:
            return printStatementTable;
        case StmtType::Read:
            return readStatementTable;
        case StmtType::If:
            return ifStatementTable;
        case StmtType::While:
            return whileStatementTable;
        case StmtType::None:
            return statementTable;
        default:
            assert(false);
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
            return modifiesStmtNameTable;
        case StmtNameRelationship::Uses:
            return usesStmtNameTable;
        default:
            assert(false);
    }
}

RelationshipTable<STMT_NUM, ENT_NAME> &PKB::getStmtNameRelationshipTable(StmtNameRelationship tableType) {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<STMT_NUM, ENT_NAME> &>(pkbPtr->getStmtNameRelationshipTable(tableType));
}

const RelationshipTable<ENT_NAME, ENT_NAME> &PKB::getNameNameRelationshipTable(NameNameRelationship tableType) const {
    switch (tableType) {
        case NameNameRelationship::Modifies:
            return modifiesNameNameTable;
        case NameNameRelationship::Uses:
            return usesNameNameTable;
        case NameNameRelationship::Calls:
            return callsNameNameTable;
        case NameNameRelationship::CallsStar:
            return callsStarNameNameTable;
        default:
            assert(false);
    }
}

RelationshipTable<STMT_NUM, STMT_NUM> &PKB::getStmtStmtRelationshipTable(StmtStmtRelationship tableType) {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<STMT_NUM, STMT_NUM> &>(pkbPtr->getStmtStmtRelationshipTable(tableType));
}

const RelationshipTable<STMT_NUM, STMT_NUM> &PKB::getStmtStmtRelationshipTable(StmtStmtRelationship tableType) const {
    switch (tableType) {
        case StmtStmtRelationship::Parent:
            return parentTable;
        case StmtStmtRelationship::ParentStar:
            return parentStarTable;
        case StmtStmtRelationship::Follows:
            return followsTable;
        case StmtStmtRelationship::FollowsStar:
            return followsStarTable;
        default:
            assert(false);
    }
}

RelationshipTable<ENT_NAME, ENT_NAME> &PKB::getNameNameRelationshipTable(NameNameRelationship tableType) {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<ENT_NAME, ENT_NAME> &>(pkbPtr->getNameNameRelationshipTable(tableType));
}

const PatternTable &PKB::getPatternTable() const {
    return patternTable;
}

PatternTable &PKB::getPatternTable() {
    const PKB *pkbPtr = const_cast<const PKB *>(this);
    return const_cast<PatternTable &>(pkbPtr->getPatternTable());
}

bool PKB::addEntityToTable(Entity entityType, ENT_NAME entity) {
    EntityTable<ENT_NAME> &table = this->getEntityTable(entityType);
    return table.addEntity(entity);
}

bool PKB::addStatementToTable(StmtType stmtType, STMT_NUM stmtNum) {
    EntityTable<STMT_NUM> &table = this->getStatementTable(stmtType);
    return table.addEntity(stmtNum);
}

bool PKB::addRelationshipToTable(StmtNameRelationship tableType, STMT_ENT stmtEnt) {
    RelationshipTable<STMT_NUM, ENT_NAME> &table = this->getStmtNameRelationshipTable(tableType);
    return table.insertPair(stmtEnt.first, stmtEnt.second);
}

bool PKB::addRelationshipToTable(NameNameRelationship tableType, ENT_ENT entEnt) {
    RelationshipTable<ENT_NAME, ENT_NAME> &table = this->getNameNameRelationshipTable(tableType);
    return table.insertPair(entEnt.first, entEnt.second);
}

bool PKB::addRelationshipToTable(StmtStmtRelationship tableType, STMT_STMT stmtStmt) {
    RelationshipTable<STMT_NUM, STMT_NUM> &table = this->getStmtStmtRelationshipTable(tableType);
    return table.insertPair(stmtStmt.first, stmtStmt.second);
}

void PKB::addPattern(STMT_NUM stmtNum, ASSIGN_PAT pattern) {
    patternTable.addPattern(stmtNum, pattern);
}

ENT_SET PKB::getEntByStmtKey(StmtNameRelationship tableType, STMT_NUM stmt) const {
    return getStmtNameRelationshipTable(tableType).getValuesByKey(stmt);
}

STMT_SET PKB::getStmtByEntVal(StmtNameRelationship tableType, ENT_NAME name) const {
    return getStmtNameRelationshipTable(tableType).getKeysByValue(name);
}

STMT_ENT_SET PKB::getStmtEntSet(StmtNameRelationship tableType) const {
    return getStmtNameRelationshipTable(tableType).getKeyValuePairs();
}

STMT_SET PKB::getStmtByRs(StmtNameRelationship tableType) const {
    return getStmtNameRelationshipTable(tableType).getKeys();
}

bool PKB::isStmtEntPairExists(StmtNameRelationship tableType, STMT_NUM stmt, ENT_NAME name) const {
    return getStmtNameRelationshipTable(tableType).containsPair(stmt, name);
}

ENT_SET PKB::getEntByEntKey(NameNameRelationship tableType, ENT_NAME name) const {
    return getNameNameRelationshipTable(tableType).getValuesByKey(name);
}

ENT_SET PKB::getEntByEntVal(NameNameRelationship tableType, ENT_NAME name) const {
    return getNameNameRelationshipTable(tableType).getKeysByValue(name);
}

ENT_ENT_SET PKB::getEntEntSet(NameNameRelationship tableType) const {
    return getNameNameRelationshipTable(tableType).getKeyValuePairs();
}

ENT_SET PKB::getKeysFrom(NameNameRelationship tableType) const {
    return getNameNameRelationshipTable(tableType).getKeys();
}

bool PKB::isEntEntPairExists(NameNameRelationship tableType, ENT_NAME key, ENT_NAME val) const {
    return getNameNameRelationshipTable(tableType).containsPair(key, val);
}

STMT_SET PKB::getStmtByStmtKey(StmtStmtRelationship tableType, STMT_NUM stmt) const {
    return getStmtStmtRelationshipTable(tableType).getValuesByKey(stmt);
}

STMT_SET PKB::getStmtByStmtVal(StmtStmtRelationship tableType, STMT_NUM stmt) const {
    return getStmtStmtRelationshipTable(tableType).getKeysByValue(stmt);
}

STMT_STMT_SET PKB::getStmtStmtSet(StmtStmtRelationship tableType) const {
    return getStmtStmtRelationshipTable(tableType).getKeyValuePairs();
}

STMT_SET PKB::getKeyStmtByRs(StmtStmtRelationship tableType) const {
    return getStmtStmtRelationshipTable(tableType).getKeys();
}

STMT_SET PKB::getValStmtByRs(StmtStmtRelationship tableType) const {
    return getStmtStmtRelationshipTable(tableType).getValues();
}

bool PKB::isStmtStmtPairExists(StmtStmtRelationship tableType, STMT_NUM key, STMT_NUM val) const {
    return getStmtStmtRelationshipTable(tableType).containsPair(key, val);
}


