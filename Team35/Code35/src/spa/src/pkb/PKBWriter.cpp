#include "PKBWriter.h"

#include <algorithm>
#include <utility>

using std::any_of;

PKBWriter::PKBWriter(PKB &pkb) : pkb(pkb) {}

bool PKBWriter::addEntities(Entity entity, const ENT_SET &entitySet) {
    for (const auto &ent : entitySet) {
        if (!pkb.addEntityToTable(entity, ent)) {
            return false;
        }
    }
    return true;
}

bool PKBWriter::addStatements(StmtType tableType, const STMT_SET &stmtSet) {
    for (const auto &stmt : stmtSet) {
        if (!pkb.addStatementToTable(tableType, stmt)) {
            return false;
        }
    }
    return true;
}

bool PKBWriter::addStmtEntityRelationships(StmtNameRelationship tableType, const STMT_ENT_SET &set) {
    for (const auto &stmtEnt : set) {
        if (!pkb.addRelationshipToTable(tableType, stmtEnt)) {
            return false;
        }
    }
    return true;
}

bool PKBWriter::addEntityEntityRelationships(NameNameRelationship tableType, const ENT_ENT_SET &set) {
    for (const auto &entEnt : set) {
        if (!pkb.addRelationshipToTable(tableType, entEnt)) {
            return false;
        }
    }
    return true;
}

bool PKBWriter::addStmtStmtRelationships(StmtStmtRelationship tableType, const STMT_STMT_SET &stmtSet) {
    for (auto stmtStmt : stmtSet) {
        if (!pkb.addRelationshipToTable(tableType, stmtStmt)) {
            return false;
        }
    }
    return true;
}

void PKBWriter::addPatterns(std::unordered_map<STMT_NUM, ASSIGN_PAT> &patMap) {
    std::unordered_map<STMT_NUM, ASSIGN_PAT>::iterator p;
    for (p = patMap.begin(); p != patMap.end(); ++p) {
        pkb.addPattern(p->first, p->second);
    }
}

void PKBWriter::addCallGraph(CallGraph &&callGraph) {
    this->addEntityEntityRelationships(NameNameRelationship::Calls, callGraph.getImmediateCalls());
    this->addEntityEntityRelationships(NameNameRelationship::CallsStar, callGraph.getTransitiveCalls());
    pkb.addCallGraph(std::move(callGraph));
}

void PKBWriter::addCFGraphs(std::vector<CFG::CFGraph> &&cfGraphs) {
    pkb.addCFGraphs(std::move(cfGraphs));
}
