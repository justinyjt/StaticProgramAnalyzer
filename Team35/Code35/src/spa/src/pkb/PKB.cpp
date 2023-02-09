#include "PKB.h"

#include <assert.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>

#include "commons/TNode.h"

const EntityTable<ENT_NAME> &PKB::getEntityTable(Entity entityType) const {
    switch (entityType) {
        case (Entity::Variable):
            return variableTable;
        case (Entity::Constant):
            return constantTable;
        default:
            assert(false);
    }
}

EntityTable<ENT_NAME> &PKB::getEntityTable(Entity entityType) {
    const PKB* pkbPtr = const_cast<const PKB *>(this);
    return const_cast<EntityTable<ENT_NAME> &>(pkbPtr->getEntityTable(entityType));
}

const RelationshipTable<STMT_NUM, ENT_NAME> &PKB::getStmtNameRelationshipTable(StmtNameRelationship tableType) const {
    switch (tableType) {
        case (StmtNameRelationship::Modifies):
            return modifiesStmtNameTable;
        case (StmtNameRelationship::Uses):
            return usesStmtNameTable;
        default:
            assert(false);
    }
}

RelationshipTable<STMT_NUM, ENT_NAME> &PKB::getStmtNameRelationshipTable(StmtNameRelationship tableType) {
    const PKB* pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<STMT_NUM, ENT_NAME> &>(pkbPtr->getStmtNameRelationshipTable(tableType));
}

const RelationshipTable<ENT_NAME, ENT_NAME> &PKB::getNameNameRelationshipTable(NameNameRelationship tableType) const {
    switch (tableType) {
        case (NameNameRelationship::Modifies):
            return modifiesNameNameTable;
        case (NameNameRelationship::Uses):
            return usesNameNameTable;
        default:
            assert(false);
    }
}

RelationshipTable<STMT_NUM, STMT_NUM> &PKB::getStmtStmtRelationshipTable(StmtStmtRelationship tableType) {
    const PKB* pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<STMT_NUM , STMT_NUM> &>(pkbPtr->getStmtStmtRelationshipTable(tableType));
}

const RelationshipTable<STMT_NUM, STMT_NUM> &PKB::getStmtStmtRelationshipTable(StmtStmtRelationship tableType) const {
    switch (tableType) {
        case (StmtStmtRelationship::Parent):
            return parentTable;
        case (StmtStmtRelationship::ParentStar):
            return parentStarTable;
        case (StmtStmtRelationship::Follows):
            return followsTable;
        case (StmtStmtRelationship::FollowsStar):
            return followsStarTable;
        default:
            assert(false);
    }
}

RelationshipTable<ENT_NAME, ENT_NAME> &PKB::getNameNameRelationshipTable(NameNameRelationship tableType) {
    const PKB* pkbPtr = const_cast<const PKB *>(this);
    return const_cast<RelationshipTable<ENT_NAME , ENT_NAME> &>(pkbPtr->getNameNameRelationshipTable(tableType));
}

const PatternTable &PKB::getPatternTable() const {
    return patternTable;
}

PatternTable &PKB::getPatternTable() {
    const PKB* pkbPtr = const_cast<const PKB *>(this);
    return const_cast<PatternTable &>(pkbPtr->getPatternTable());
}

bool PKB::addEntityToTable(Entity entityType, ENT_NAME entity) {
    EntityTable<ENT_NAME> &table = this->getEntityTable(entityType);
    return table.addEntity(entity);
}
bool PKB::addRelationshipToTable(StmtNameRelationship tableType, STMT_ENT stmtEnt) {
    RelationshipTable<STMT_NUM, ENT_NAME> &table = this->getStmtNameRelationshipTable(tableType);
    return table.insertPair(stmtEnt.first, stmtEnt.second);
}
bool PKB::addRelationshipToTable(NameNameRelationship tableType, ENT_ENT entEnt) {
    RelationshipTable<ENT_NAME , ENT_NAME> &table = this->getNameNameRelationshipTable(tableType);
    return table.insertPair(entEnt.first, entEnt.second);
}
bool PKB::addPattern(STMT_NUM stmtNum, std::string pattern) {
    return patternTable.addPattern(stmtNum, pattern);
}

ENT_SET PKB::getEntByStmtKey(StmtNameRelationship tableType, STMT_NUM stmt) const {
    return getStmtNameRelationshipTable(tableType).getValues(stmt);
}

STMT_SET PKB::getStmtByEntVal(StmtNameRelationship tableType, ENT_NAME name) const {
    return getStmtNameRelationshipTable(tableType).getKeys(name);
}

STMT_ENT_SET PKB::getStmtEntSet(StmtNameRelationship tableType) const {
    return getStmtNameRelationshipTable(tableType).getKeyValuePairs();
}

bool PKB::isStmtEntPairExists(StmtNameRelationship tableType, STMT_NUM stmt, ENT_NAME name) const {
    return getStmtNameRelationshipTable(tableType).containsPair(stmt, name);
}

ENT_SET PKB::getEntByEntKey(NameNameRelationship tableType, ENT_NAME name) const {
    return getNameNameRelationshipTable(tableType).getValues(name);
}

ENT_SET PKB::getEntByEntVal(NameNameRelationship tableType, ENT_NAME name) const {
    return getNameNameRelationshipTable(tableType).getKeys(name);
}

ENT_ENT_SET PKB::getEntEntSet(NameNameRelationship tableType) const {
    return getNameNameRelationshipTable(tableType).getKeyValuePairs();
}

bool PKB::isEntEntPairExists(NameNameRelationship tableType, ENT_NAME key, ENT_NAME val) const {
    return getNameNameRelationshipTable(tableType).containsPair(key, val);
}

STMT_SET PKB::getStmtByStmtKey(StmtStmtRelationship tableType, STMT_NUM stmt) const {
    return getStmtStmtRelationshipTable(tableType).getValues(stmt);
}

STMT_SET PKB::getStmtByStmtVal(StmtStmtRelationship tableType, STMT_NUM stmt) const {
    return getStmtStmtRelationshipTable(tableType).getKeys(stmt);
}
STMT_STMT_SET PKB::getStmtStmtSet(StmtStmtRelationship tableType) const {
    return getStmtStmtRelationshipTable(tableType).getKeyValuePairs();
}

bool PKB::isStmtStmtPairExists(StmtStmtRelationship tableType, STMT_NUM key, STMT_NUM val) const {
    return getStmtStmtRelationshipTable(tableType).containsPair(key, val);
}


