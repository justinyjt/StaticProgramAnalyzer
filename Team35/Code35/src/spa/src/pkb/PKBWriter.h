#pragma once

#include "PKB.h"

#include <string>
#include <unordered_map>

class PKBWriter {
 public:
    explicit PKBWriter(PKB &);

    bool addEntities(Entity entity, ENT_SET entitySet);

    bool addStatements(StmtType tableType, STMT_SET stmtSet);

    bool addStmtStmtRelationships(StmtStmtRelationship tableType, STMT_STMT_SET set);

    bool addStmtEntityRelationships(StmtNameRelationship tableType, STMT_ENT_SET set);

    bool addEntityEntityRelationships(NameNameRelationship tableType, ENT_ENT_SET set);

    bool addPatterns(std::unordered_map<STMT_NUM, ASSIGN_PAT> patMap);

 private:
    PKB &pkb;
};
