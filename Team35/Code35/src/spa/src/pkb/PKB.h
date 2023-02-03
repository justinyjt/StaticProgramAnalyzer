#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "commons/types.h"
#include "pkb/db/EntityTable.h"
#include "pkb/db/RelationshipTable.h"
#include "pkb/db/PatternTable.h"

typedef int PROC;

class TNode;

class PKB {
 public:
    const EntityTable<ENT_NAME> &getEntityTable(Entity entityType) const;
    const RelationshipTable<STMT_NUM, ENT_NAME> &getStmtNameRelationshipTable(StmtNameRelationship tableType) const;
    const RelationshipTable<ENT_NAME, ENT_NAME> &getNameNameRelationshipTable(NameNameRelationship tableType) const;
    const PatternTable &getPatternTable() const;

    RelationshipTable<STMT_NUM, ENT_NAME> &getStmtNameRelationshipTable(StmtNameRelationship tableType);
    RelationshipTable<ENT_NAME, ENT_NAME> &getNameNameRelationshipTable(NameNameRelationship tableType);
    EntityTable<ENT_NAME> &getEntityTable(Entity entityType);
    PatternTable &getPatternTable();
    bool addEntityToTable(Entity entityType, ENT_NAME entity);
    bool addRelationshipToTable(StmtNameRelationship tableType, STMT_ENT stmtEnt);
    bool addRelationshipToTable(NameNameRelationship tableType, ENT_ENT entEnt);
    bool addPattern(STMT_NUM stmtNum, std::string pattern);

 private:
    RelationshipTable<STMT_NUM, ENT_NAME> modifiesStmtNameTable;
    RelationshipTable<STMT_NUM, ENT_NAME> usesStmtNameTable;
    RelationshipTable<ENT_NAME , ENT_NAME> modifiesNameNameTable;
    RelationshipTable<ENT_NAME , ENT_NAME> usesNameNameTable;
    EntityTable<ENT_NAME> variableTable;
    EntityTable<ENT_NAME> constantTable;
    PatternTable patternTable;
};
