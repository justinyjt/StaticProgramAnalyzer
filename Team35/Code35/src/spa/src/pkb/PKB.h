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

    const EntityTable<STMT_NUM> &getStatementTable(StmtType stmtType) const;

    const RelationshipTable<STMT_NUM, ENT_NAME> &getStmtNameRelationshipTable(StmtNameRelationship tableType) const;

    const RelationshipTable<ENT_NAME, ENT_NAME> &getNameNameRelationshipTable(NameNameRelationship tableType) const;

    const RelationshipTable<STMT_NUM, STMT_NUM> &getStmtStmtRelationshipTable(StmtStmtRelationship tableType) const;

    const PatternTable &getPatternTable() const;

    RelationshipTable<STMT_NUM, ENT_NAME> &getStmtNameRelationshipTable(StmtNameRelationship tableType);

    RelationshipTable<ENT_NAME, ENT_NAME> &getNameNameRelationshipTable(NameNameRelationship tableType);

    RelationshipTable<STMT_NUM, STMT_NUM> &getStmtStmtRelationshipTable(StmtStmtRelationship tableType);

    EntityTable<ENT_NAME> &getEntityTable(Entity entityType);

    EntityTable<STMT_NUM> &getStatementTable(StmtType stmtType);

    PatternTable &getPatternTable();

    bool addEntityToTable(Entity entityType, ENT_NAME entity);

    bool addStatementToTable(StmtType stmtType, STMT_NUM stmt);

    bool addRelationshipToTable(StmtNameRelationship tableType, STMT_ENT stmtEnt);

    bool addRelationshipToTable(NameNameRelationship tableType, ENT_ENT entEnt);

    bool addRelationshipToTable(StmtStmtRelationship tableType, STMT_STMT stmtStmt);

    bool addPattern(STMT_NUM stmt, ASSIGN_PAT pattern);

    ENT_SET getEntByStmtKey(StmtNameRelationship tableType, STMT_NUM stmt) const;

    STMT_SET getStmtByEntVal(StmtNameRelationship tableType, ENT_NAME name) const;

    STMT_ENT_SET getStmtEntSet(StmtNameRelationship tableType) const;

    STMT_SET getStmtByRs(StmtNameRelationship tableType) const;

    bool isStmtEntPairExists(StmtNameRelationship tableType, STMT_NUM stmt, ENT_NAME name) const;

    ENT_SET getEntByEntKey(NameNameRelationship tableType, ENT_NAME name) const;

    ENT_SET getEntByEntVal(NameNameRelationship tableType, ENT_NAME name) const;

    ENT_ENT_SET getEntEntSet(NameNameRelationship tableType) const;

    bool isEntEntPairExists(NameNameRelationship tableType, ENT_NAME key, ENT_NAME val) const;

    STMT_SET getStmtByStmtKey(StmtStmtRelationship tableType, STMT_NUM stmt) const;

    STMT_SET getStmtByStmtVal(StmtStmtRelationship tableType, STMT_NUM stmt) const;

    STMT_STMT_SET getStmtStmtSet(StmtStmtRelationship tableType) const;

    STMT_SET getKeyStmtByRs(StmtStmtRelationship tableType) const;

    STMT_SET getValStmtByRs(StmtStmtRelationship tableType) const;

    bool isStmtStmtPairExists(StmtStmtRelationship tableType, STMT_NUM key, STMT_NUM val) const;


 private:
    // Entity related tables
    EntityTable<ENT_NAME> variableTable;
    EntityTable<ENT_NAME> constantTable;
    EntityTable<ENT_NAME> procedureTable;
    EntityTable<STMT_NUM> assignStatementTable;
    EntityTable<STMT_NUM> ifStatementTable;
    EntityTable<STMT_NUM> whileStatementTable;
    EntityTable<STMT_NUM> printStatementTable;
    EntityTable<STMT_NUM> readStatementTable;
    EntityTable<STMT_NUM> callStatementTable;
    EntityTable<STMT_NUM> statementTable;

    // Relationship related tables
    RelationshipTable<STMT_NUM, ENT_NAME> modifiesStmtNameTable;
    RelationshipTable<STMT_NUM, ENT_NAME> usesStmtNameTable;
    RelationshipTable<ENT_NAME, ENT_NAME> modifiesNameNameTable;
    RelationshipTable<ENT_NAME, ENT_NAME> usesNameNameTable;
    RelationshipTable<ENT_NAME, ENT_NAME> callsNameNameTable;
    RelationshipTable<ENT_NAME, ENT_NAME> callsStarNameNameTable;

    RelationshipTable<STMT_NUM, STMT_NUM> followsTable;
    RelationshipTable<STMT_NUM, STMT_NUM> followsStarTable;
    RelationshipTable<STMT_NUM, STMT_NUM> parentTable;
    RelationshipTable<STMT_NUM, STMT_NUM> parentStarTable;

    PatternTable patternTable;
};
