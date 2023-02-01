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

 private:
    RelationshipTable<STMT_NUM, ENT_NAME> modifiesStmtNameTable;
    RelationshipTable<STMT_NUM, ENT_NAME> usesStmtNameTable;
    EntityTable<ENT_NAME> variableTable;
    EntityTable<ENT_NAME> constantTable;
    PatternTable patternTable;
};
