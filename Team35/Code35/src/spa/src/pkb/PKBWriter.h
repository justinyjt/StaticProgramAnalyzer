#pragma once

#include "PKB.h"

#include <string>
#include <unordered_map>
#include <vector>

#include "commons/graph/CallGraph.h"

class PKBWriter {
 public:
    explicit PKBWriter(PKB &pkb);

    bool addEntities(Entity entity, const ENT_SET &entitySet);

    bool addStatements(StmtType tableType, const STMT_SET &stmtSet);

    bool addStmtStmtRelationships(StmtStmtRelationship tableType, const STMT_STMT_SET &set);

    bool addStmtEntityRelationships(StmtNameRelationship tableType, const STMT_ENT_SET &set);

    bool addEntityEntityRelationships(NameNameRelationship tableType, const ENT_ENT_SET &set);

    void addPatterns(std::unordered_map<STMT_NUM, ASSIGN_PAT> &patMap);

    void addCallGraph(CallGraph &&callGraph);

    void addCFGraphs(std::vector<CFG::CFGraph> &&cfGraphs);

 private:
    PKB &pkb;
};
