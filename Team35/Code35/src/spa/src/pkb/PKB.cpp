#include "PKB.h"

#include <assert.h>
#include<stdio.h>
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

const PatternTable &PKB::getPatternTable() const {
    return patternTable;
}

PatternTable &PKB::getPatternTable() {
    const PKB* pkbPtr = const_cast<const PKB *>(this);
    return const_cast<PatternTable &>(pkbPtr->getPatternTable());
}


