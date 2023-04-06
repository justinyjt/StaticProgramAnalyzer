#pragma once

#include <string>
#include <unordered_set>

#include "PKB.h"
#include "pkb/db/AffectsGraph.h"

class PKBReader {
 public:
    explicit PKBReader(PKB &);

    [[nodiscard]] ENT_SET getEntities(Entity entityType) const;

    [[nodiscard]] STMT_SET getStatements(StmtType stmtType) const;

    [[nodiscard]] ENT_SET getRelationship(StmtNameRelationship tableType, STMT_NUM stmt) const;

    [[nodiscard]] STMT_SET getRelationship(StmtNameRelationship tableType, const ENT_NAME &name) const;

    [[nodiscard]] STMT_SET
    getRelationshipWithFilter(StmtNameRelationship tableType, const ENT_NAME &name, StmtType stmtType) const;

    [[nodiscard]] STMT_ENT_SET getAllRelationships(StmtNameRelationship tableType) const;

    [[nodiscard]] STMT_ENT_SET getAllRelationshipsWithFilter(StmtNameRelationship tableType, StmtType stmtType) const;

    [[nodiscard]] STMT_SET getStmtByRelationship(StmtNameRelationship tableType) const;

    [[nodiscard]] STMT_SET getStmtByRelationshipWithFilter(StmtNameRelationship tableType, StmtType stmtType) const;

    [[nodiscard]] bool isRelationshipExists(StmtNameRelationship tableType, STMT_NUM stmt, const ENT_NAME &name) const;

    [[nodiscard]] ENT_SET getRelationshipByKey(NameNameRelationship tableType, const ENT_NAME &keyName) const;

    [[nodiscard]] ENT_SET getRelationshipByVal(NameNameRelationship tableType, const ENT_NAME &valName) const;

    [[nodiscard]] ENT_ENT_SET getAllRelationships(NameNameRelationship tableType) const;

    [[nodiscard]] ENT_SET getKeyNameByRelationship(NameNameRelationship tableType) const;

    [[nodiscard]] ENT_SET getValueNameByRelationship(NameNameRelationship tableType) const;

    [[nodiscard]] bool
    isRelationshipExists(NameNameRelationship tableType, const ENT_NAME &keyName, const ENT_NAME &valName) const;

    STMT_SET getRelationshipByKey(StmtStmtRelationship tableType, STMT_NUM keyName);

    STMT_SET getRelationshipByVal(StmtStmtRelationship tableType, STMT_NUM valName);

    STMT_SET getRelationshipByStmtWithFilter(StmtStmtRelationship tableType,
                                             STMT_NUM stmt,
                                             StmtType stmtType,
                                             bool isKey);

    STMT_STMT_SET getAllRelationshipsWithFilter(StmtStmtRelationship tableType, StmtType first, StmtType second);

    STMT_STMT_SET getAllRelationshipsWithFilter(StmtStmtRelationship tableType, StmtType stmtType);

    STMT_SET getKeyStmtByRelationship(StmtStmtRelationship tableType);

    STMT_SET getValueStmtByRelationship(StmtStmtRelationship tableType);

    STMT_SET getStmtByRelationshipWithFilter(StmtStmtRelationship tableType, StmtType stmtType, bool isKey);

    bool isRelationshipExists(StmtStmtRelationship tableType, STMT_NUM keyName, STMT_NUM valName);

    bool hasRelationship(StmtStmtRelationship tableType);

    STMT_SET getStmtWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_SET getStmtWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_ENT_SET getStmtVarExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_ENT_SET getStmtVarPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_SET getStmtWithExactPatternIntersect(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT &right) const;

    STMT_SET getStmtWithPartialPatternIntersect(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT &right) const;

    void clearCache();

 private:
    PKB &pkb;
    AffectsGraph affects_graph_;
    bool isAffectsComputed;

    [[nodiscard]] bool isAffects(STMT_NUM stmt1, STMT_NUM stmt2) const;

    bool isAffectsT(STMT_NUM first, STMT_NUM second);

    bool isValidAffectsSuccessor(STMT_NUM stmt);

    bool isValidAffectsPredecessor(STMT_NUM stmt);

    STMT_SET getAffectsBySuccessor(STMT_NUM stmt2, bool isTransitive);

    STMT_SET getAffectsByPredecessor(STMT_NUM stmt1, bool isTransitive);

    STMT_STMT_SET getAllAffects(bool isTransitive);

    STMT_SET getAllAffectsPredecessors();

    STMT_SET getAllAffectsSuccessors();

    STMT_SET getIntersect(STMT_NUM first, STMT_NUM second);

    bool hasAffects();

    [[nodiscard]] bool isModifies(STMT_NUM key, const ENT_NAME &val) const;

    STMT_STMT_SET getAllRelationships(StmtStmtRelationship tableType);

    bool isContainerStmt(STMT_NUM num) const;

    bool isSuccessorCandidate(STMT_NUM num, ENT_SET modifies) const;

    bool isAffectsHelper(STMT_NUM start, STMT_NUM end, ENT_SET modifies) const;

    void computeAffectsGraph();
};
