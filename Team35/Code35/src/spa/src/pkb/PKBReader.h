#ifndef TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBREADER_H_
#define TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBREADER_H_

#include "PKB.h"

#include <string>

class PKBReader {
 public:
    explicit PKBReader(PKB &);

    ENT_SET getEntities(Entity entityType) const;

    STMT_SET getStatements(StmtType stmtType) const;

    ENT_SET getRelationship(StmtNameRelationship tableType, STMT_NUM stmt) const;

    STMT_SET getRelationship(StmtNameRelationship tableType, ENT_NAME name) const;

    STMT_ENT_SET getAllRelationships(StmtNameRelationship tableType) const;

    STMT_SET getStmtByRelationship(StmtNameRelationship tableType) const;

    bool isRelationshipExists(StmtNameRelationship tableType, STMT_NUM stmt, ENT_NAME name) const;

    ENT_SET getRelationshipByKey(NameNameRelationship tableType, ENT_NAME keyName) const;

    ENT_SET getRelationshipByVal(NameNameRelationship tableType, ENT_NAME valName) const;

    ENT_ENT_SET getAllRelationships(NameNameRelationship tableType) const;

    bool isRelationshipExists(NameNameRelationship tableType, ENT_NAME keyName, ENT_NAME valName) const;

    STMT_SET getRelationshipByKey(StmtStmtRelationship tableType, STMT_NUM keyName) const;

    STMT_SET getRelationshipByVal(StmtStmtRelationship tableType, STMT_NUM valName) const;

    STMT_STMT_SET getAllRelationships(StmtStmtRelationship tableType) const;

    STMT_SET getKeyStmtByRelationship(StmtStmtRelationship tableType) const;

    STMT_SET getValueStmtByRelationship(StmtStmtRelationship tableType) const;

    STMT_SET getStmtByRs(StmtStmtRelationship tableType) const;

    bool isRelationshipExists(StmtStmtRelationship tableType, STMT_NUM keyName, STMT_NUM valName) const;

    STMT_SET getStmtWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;
//    bool isExactPatternMatch(STMT_NUM stmtNo, std::string &pattern) const;

    STMT_SET getStmtWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

 private:
    PKB &pkb;
};


#endif  // TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBREADER_H_
