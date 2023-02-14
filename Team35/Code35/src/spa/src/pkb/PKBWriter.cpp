#include "PKBWriter.h"

PKBWriter::PKBWriter(PKB &pkb) : pkb(pkb) {}

bool PKBWriter::addEntities(Entity entity, ENT_SET entitySet) {
    for (auto ent : entitySet) {
        if (!pkb.addEntityToTable(entity, ent)) {
            return false;
        }
    }
    return true;
}
bool PKBWriter::addStatementEntities(StmtType tableType, STMT_SET stmtSet) {
    for (auto stmt : stmtSet) {
        if (!pkb.addStatementToTable(tableType, stmt)) {
            return false;
        }
    }
    return true;
}

bool PKBWriter::addStmtEntityRelationships(StmtNameRelationship tableType, STMT_ENT_SET set) {
    for (auto stmtEnt : set) {
        if (!pkb.addRelationshipToTable(tableType, stmtEnt)) {
            return false;
        }
    }
    return true;
}

bool PKBWriter::addEntityEntityRelationships(NameNameRelationship tableType, ENT_ENT_SET set) {
    for (auto entEnt : set) {
        if (!pkb.addRelationshipToTable(tableType, entEnt)) {
            return false;
        }
    }
    return true;
}

bool PKBWriter::addStmtStmtRelationships(StmtStmtRelationship tableType, STMT_STMT_SET stmtSet) {
    for (auto stmtStmt : stmtSet) {
        if (!pkb.addRelationshipToTable(tableType, stmtStmt)) {
            return false;
        }
    }
    return true;
}


bool PKBWriter::addPatterns(std::unordered_map<STMT_NUM, std::string> patternMap) {
    std::unordered_map<STMT_NUM , std::string>:: iterator p;
    for (p = patternMap.begin(); p != patternMap.end(); p++) {
        if (!pkb.addPattern(p->first, p -> second)) {
            return false;
        }
    }
    return true;
}
