#pragma once

#include "PKB.h"

#include <string>
#include <unordered_map>

class Writer {
 public:
    explicit Writer(PKB &);

    bool addEntities(Entity entity, ENT_SET entitySet);
    bool addStmtEntityRelationships(StmtNameRelationship tableType, STMT_ENT_SET set);
    bool addEntityEntityRelationships(StmtNameRelationship tableType, ENT_ENT_SET set);
    bool addPatterns(std::unordered_map<STMT_NUM, std::string> patternMap);

 private:
    PKB &pkb;
};
