#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "commons/types.h"
#include "pkb/db/EntityTable.h"
#include "pkb/db/RelationshipTable.h"
#include "pkb/db/PatternTable.h"
#include "pkb/db/CFGManager.h"
#include "commons/graph/CallGraph.h"

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

    bool addRelationshipToTable(StmtNameRelationship tableType, const STMT_ENT &stmtEnt);

    bool addRelationshipToTable(NameNameRelationship tableType, const ENT_ENT &entEnt);

    bool addRelationshipToTable(StmtStmtRelationship tableType, STMT_STMT stmtStmt);

    void addPattern(STMT_NUM stmt, ASSIGN_PAT pattern);

    void addCallGraph(CallGraph &&callGraph);

    void addCFGraphs(std::vector<CFG::CFGraph> &&CFGraphs);

    ENT_SET getEntByStmtKey(StmtNameRelationship tableType, STMT_NUM stmt) const;

    STMT_SET getStmtByEntVal(StmtNameRelationship tableType, const ENT_NAME &name) const;

    STMT_ENT_SET getStmtEntSet(StmtNameRelationship tableType) const;

    STMT_SET getStmtByRs(StmtNameRelationship tableType) const;

    bool isStmtEntPairExists(StmtNameRelationship tableType, STMT_NUM stmt, const ENT_NAME &name) const;

    ENT_SET getEntByEntKey(NameNameRelationship tableType, const ENT_NAME &name) const;

    ENT_SET getEntByEntVal(NameNameRelationship tableType, const ENT_NAME &name) const;

    ENT_ENT_SET getEntEntSet(NameNameRelationship tableType) const;

    ENT_SET getKeyNameByRs(NameNameRelationship tableType) const;

    ENT_SET getValNameByRs(NameNameRelationship tableType) const;

    bool isEntEntPairExists(NameNameRelationship tableType, const ENT_NAME &key, const ENT_NAME &val) const;

    STMT_SET getStmtByStmtKey(StmtStmtRelationship tableType, STMT_NUM stmt) const;

    STMT_SET getStmtByStmtVal(StmtStmtRelationship tableType, STMT_NUM stmt) const;

    STMT_SET getStmtByProc(const ENT_NAME &procName) const;

    STMT_STMT_SET getStmtStmtSet(StmtStmtRelationship tableType);

    STMT_SET getKeyStmtByRs(StmtStmtRelationship tableType) const;

    STMT_SET getValStmtByRs(StmtStmtRelationship tableType) const;

    bool isStmtStmtPairExists(StmtStmtRelationship tableType, STMT_NUM key, STMT_NUM val) const;

    bool isEntityTypeExists(StmtType tableType, STMT_NUM key) const;

    bool isRelationshipExists(StmtStmtRelationship tableType);

 private:
    // Entity related tables
    EntityTable<ENT_NAME> variableTable_;
    EntityTable<ENT_NAME> constantTable_;
    EntityTable<ENT_NAME> procedureTable_;
    EntityTable<STMT_NUM> assignStatementTable_;
    EntityTable<STMT_NUM> ifStatementTable_;
    EntityTable<STMT_NUM> whileStatementTable_;
    EntityTable<STMT_NUM> printStatementTable_;
    EntityTable<STMT_NUM> readStatementTable_;
    EntityTable<STMT_NUM> callStatementTable_;
    EntityTable<STMT_NUM> statementTable_;

    // Relationship related tables
    RelationshipTable<STMT_NUM, ENT_NAME> modifiesStmtNameTable_;
    RelationshipTable<STMT_NUM, ENT_NAME> usesStmtNameTable_;
    RelationshipTable<STMT_NUM, ENT_NAME> ifCondUsesVarTable_;
    RelationshipTable<STMT_NUM, ENT_NAME> whileCondUsesVarTable_;
    RelationshipTable<STMT_NUM, ENT_NAME> containerProcedureTable_;
    RelationshipTable<STMT_NUM, ENT_NAME> readStmtVarTable_;
    RelationshipTable<STMT_NUM, ENT_NAME> printStmtVarTable_;
    RelationshipTable<STMT_NUM, ENT_NAME> callsStmtProcedureTable_;

    RelationshipTable<ENT_NAME, ENT_NAME> modifiesNameNameTable_;
    RelationshipTable<ENT_NAME, ENT_NAME> usesNameNameTable_;
    RelationshipTable<ENT_NAME, ENT_NAME> callsNameNameTable_;
    RelationshipTable<ENT_NAME, ENT_NAME> callsStarNameNameTable_;

    RelationshipTable<STMT_NUM, STMT_NUM> followsTable_;
    RelationshipTable<STMT_NUM, STMT_NUM> followsStarTable_;
    RelationshipTable<STMT_NUM, STMT_NUM> parentTable_;
    RelationshipTable<STMT_NUM, STMT_NUM> parentStarTable_;

    PatternTable patternTable_;
    CallGraph callGraph_;
    CFG::CFGManager cfgManager_;
};
