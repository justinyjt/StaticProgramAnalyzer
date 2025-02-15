#pragma once

#include <cstdio>
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
    [[nodiscard]] const EntityTable<ENT_NAME> &getEntityTable(Entity entityType) const;

    [[nodiscard]] const EntityTable<STMT_NUM> &getStatementTable(StmtType stmtType) const;

    [[nodiscard]] const RelationshipTable<STMT_NUM, ENT_NAME> &
    getStmtNameRelationshipTable(StmtNameRelationship tableType) const;

    [[nodiscard]] const RelationshipTable<ENT_NAME, ENT_NAME> &
    getNameNameRelationshipTable(NameNameRelationship tableType) const;

    [[nodiscard]] const RelationshipTable<STMT_NUM, STMT_NUM> &
    getStmtStmtRelationshipTable(StmtStmtRelationship tableType) const;

    [[nodiscard]] const PatternTable &getPatternTable() const;

    RelationshipTable<STMT_NUM, ENT_NAME> &getStmtNameRelationshipTable(StmtNameRelationship tableType);

    RelationshipTable<ENT_NAME, ENT_NAME> &getNameNameRelationshipTable(NameNameRelationship tableType);

    RelationshipTable<STMT_NUM, STMT_NUM> &getStmtStmtRelationshipTable(StmtStmtRelationship tableType);

    EntityTable<ENT_NAME> &getEntityTable(Entity entityType);

    EntityTable<STMT_NUM> &getStatementTable(StmtType stmtType);

    PatternTable &getPatternTable();

    void addEntityToTable(Entity entityType, ENT_NAME entity);

    void addStatementToTable(StmtType stmtType, STMT_NUM stmt);

    void addRelationshipToTable(StmtNameRelationship tableType, const STMT_ENT &stmtEnt);

    void addRelationshipToTable(NameNameRelationship tableType, const ENT_ENT &entEnt);

    void addRelationshipToTable(StmtStmtRelationship tableType, STMT_STMT stmtStmt);

    void addPattern(STMT_NUM stmt, ASSIGN_PAT pattern);

    void addCallGraph(CallGraph &&callGraph);

    void addCFGraphs(std::vector<CFG::CFGraph> &&CFGraphs);

    [[nodiscard]] ENT_SET getEntByStmtKey(StmtNameRelationship tableType, STMT_NUM stmt) const;

    [[nodiscard]] STMT_SET getStmtByEntVal(StmtNameRelationship tableType, const ENT_NAME &name) const;

    [[nodiscard]] STMT_ENT_SET getStmtEntSet(StmtNameRelationship tableType) const;

    [[nodiscard]] STMT_SET getStmtByRs(StmtNameRelationship tableType) const;

    [[nodiscard]] bool isStmtEntPairExists(StmtNameRelationship tableType, STMT_NUM stmt, const ENT_NAME &name) const;

    [[nodiscard]] ENT_SET getEntByEntKey(NameNameRelationship tableType, const ENT_NAME &name) const;

    [[nodiscard]] ENT_SET getEntByEntVal(NameNameRelationship tableType, const ENT_NAME &name) const;

    [[nodiscard]] ENT_ENT_SET getEntEntSet(NameNameRelationship tableType) const;

    [[nodiscard]] ENT_SET getKeyNameByRs(NameNameRelationship tableType) const;

    [[nodiscard]] ENT_SET getValNameByRs(NameNameRelationship tableType) const;

    [[nodiscard]] bool
    isEntEntPairExists(NameNameRelationship tableType, const ENT_NAME &key, const ENT_NAME &val) const;

    [[nodiscard]] STMT_SET getStmtByStmtKey(StmtStmtRelationship tableType, STMT_NUM stmt) const;

    [[nodiscard]] STMT_SET getStmtByStmtVal(StmtStmtRelationship tableType, STMT_NUM stmt) const;

    STMT_STMT_SET getStmtStmtSet(StmtStmtRelationship tableType);

    [[nodiscard]] STMT_SET getKeyStmtByRs(StmtStmtRelationship tableType) const;

    [[nodiscard]] STMT_SET getValStmtByRs(StmtStmtRelationship tableType) const;

    [[nodiscard]] bool isStmtStmtPairExists(StmtStmtRelationship tableType, STMT_NUM key, STMT_NUM val) const;

    [[nodiscard]] bool isEntityTypeExists(StmtType tableType, STMT_NUM key) const;

    bool isRelationshipExists(StmtStmtRelationship tableType);

    STMT_SET getStmtWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_SET getStmtWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_ENT_SET getStmtVarWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_ENT_SET getStmtVarWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_SET getStmtIntersectWithExactPatternMatch(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT &right) const;

    STMT_SET getStmtIntersectWithPartialPatternMatch(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT &right) const;

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
