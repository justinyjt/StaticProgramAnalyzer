#pragma once

#include "PKB.h"

#include <string>
#include <unordered_map>

class PKBWriter {
 public:
    explicit PKBWriter(PKB &);

    bool addEntities(Entity entity, ENT_SET entitySet);
    bool addStatementEntities(StmtType tableType, STMT_SET set);
    bool addStmtStmtRelationships(StmtStmtRelationship tableType, STMT_STMT_SET set);
    bool addStmtEntityRelationships(StmtNameRelationship tableType, STMT_ENT_SET set);
    bool addEntityEntityRelationships(NameNameRelationship tableType, ENT_ENT_SET set);
    bool addPatterns(std::unordered_map<STMT_NUM, std::string> patternMap);

 private:
    PKB &pkb;
};
