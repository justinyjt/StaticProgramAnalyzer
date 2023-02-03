#include "Writer.h"

Writer::Writer(PKB &pkb) : pkb(pkb) {}

bool Writer::addEntities(Entity entity, ENT_SET entitySet) {
    for (auto ent : entitySet) {
        if (!pkb.addEntityToTable(entity, ent)) {
            return false;
        }
    }
    return true;
}

bool Writer::addStmtEntityRelationships(StmtNameRelationship tableType, STMT_ENT_SET set) {
    for (auto stmtEnt : set) {
        if (!pkb.addRelationshipToTable(tableType, stmtEnt)) {
            return false;
        }
    }
    return true;
}

bool Writer::addPatterns(std::unordered_map<STMT_NUM, std::string> patternMap) {
    std::unordered_map<STMT_NUM , std::string>:: iterator p;
    for (p = patternMap.begin(); p != patternMap.end(); p++) {
        if (!pkb.addPattern(p->first, p -> second)) {
            return false;
        }
    }
    return true;
}
