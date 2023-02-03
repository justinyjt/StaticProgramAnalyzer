#ifndef TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBREADER_H_
#define TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBREADER_H_

#include "PKB.h"

#include <string>

class PKBReader {
 public:
    explicit PKBReader(PKB &);

    ENT_SET getEntities(Entity entityType) const;

    ENT_SET getRelationship(StmtNameRelationship tableType, STMT_NUM stmt) const;
    STMT_SET getRelationship(StmtNameRelationship tableType, ENT_NAME name) const;
    STMT_ENT_SET getAllRelationships(StmtNameRelationship tableType) const;
    bool isRelationshipExists(StmtNameRelationship tableType, STMT_NUM stmt, ENT_NAME name) const;

    ENT_SET getRelationshipByKey(NameNameRelationship tableType, ENT_NAME keyName) const;
    ENT_SET getRelationshipByVal(NameNameRelationship tableType, ENT_NAME valName) const;
    ENT_ENT_SET getAllRelationships(NameNameRelationship tableType) const;
    bool isRelationshipExists(NameNameRelationship tableType, ENT_NAME keyName, ENT_NAME valName) const;

    STMT_SET getStmtWithExactPatternMatch(std::string &pattern) const;
//    bool isExactPatternMatch(STMT_NUM stmtNo, std::string &pattern) const;

 private:
    PKB &pkb;
};


#endif  // TEAM35_CODE35_SRC_SPA_SRC_PKB_PKBREADER_H_
