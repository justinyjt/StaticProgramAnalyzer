#pragma once

#include <string>

#include "PKB.h"
#include "pkb/db/AffectsGraph.h"

class PKBReader {
 public:
    explicit PKBReader(PKB &);

    ENT_SET getEntities(Entity entityType) const;

    STMT_SET getStatements(StmtType stmtType) const;

    ENT_SET getRelationship(StmtNameRelationship tableType, STMT_NUM stmt) const;

    STMT_SET getRelationship(StmtNameRelationship tableType, const ENT_NAME &name) const;

    STMT_ENT_SET getAllRelationships(StmtNameRelationship tableType) const;

    STMT_SET getStmtByRelationship(StmtNameRelationship tableType) const;

    bool isRelationshipExists(StmtNameRelationship tableType, STMT_NUM stmt, const ENT_NAME &name) const;

    ENT_SET getRelationshipByKey(NameNameRelationship tableType, const ENT_NAME &keyName) const;

    ENT_SET getRelationshipByVal(NameNameRelationship tableType, const ENT_NAME &valName) const;

    ENT_ENT_SET getAllRelationships(NameNameRelationship tableType) const;

    ENT_SET getKeyNameByRelationship(NameNameRelationship tableType) const;

    ENT_SET getValueNameByRelationship(NameNameRelationship tableType) const;

    bool isRelationshipExists(NameNameRelationship tableType, const ENT_NAME &keyName, const ENT_NAME &valName) const;

    STMT_SET getRelationshipByKey(StmtStmtRelationship tableType, STMT_NUM keyName);

    STMT_SET getRelationshipByVal(StmtStmtRelationship tableType, STMT_NUM valName);

    STMT_STMT_SET getAllRelationships(StmtStmtRelationship tableType);

    STMT_SET getKeyStmtByRelationship(StmtStmtRelationship tableType);

    STMT_SET getValueStmtByRelationship(StmtStmtRelationship tableType);

    STMT_SET getStmtByRs(StmtStmtRelationship tableType);

    STMT_SET getStmtByProc(const ENT_NAME &procName) const;

    bool isRelationshipExists(StmtStmtRelationship tableType, STMT_NUM keyName, STMT_NUM valName);

    STMT_SET getStmtWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_SET getStmtWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

 private:
    PKB &pkb;
    AffectsGraph affects_graph_;

    bool isAffects(STMT_NUM stmt1, STMT_NUM stmt2) const;

    bool isAffectsT(STMT_NUM first, STMT_NUM second);

    bool isValidAffectsSuccessor(STMT_NUM stmt);

    bool isValidAffectsPredecessor(STMT_NUM stmt);

    STMT_SET getAffectsBySuccessor(STMT_NUM stmt2, bool isTransitive);

    STMT_SET getAffectsByPredecessor(STMT_NUM stmt1, bool isTransitive);

    STMT_STMT_SET getAllAffects(bool isTransitive);

    STMT_SET getAllAffectsPredecessors();

    STMT_SET getAllAffectsSuccessors();

    STMT_SET getIntersect(STMT_NUM first, STMT_NUM second) const;

    bool hasAffects();

    bool isModifies(STMT_NUM key, const ENT_NAME &val) const;
};
