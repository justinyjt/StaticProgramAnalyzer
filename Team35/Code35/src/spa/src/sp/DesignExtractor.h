#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "commons/ASTNode.h"
#include "commons/graph/CallGraph.h"
#include "commons/graph/CFGraph.h"
#include "pkb/PKBWriter.h"
#include "CFGraphBuilder.h"

struct ProcNode {
    ENT_NAME procName;
    std::vector<ENT_NAME> path;

    ProcNode(ENT_NAME name, const std::vector<ENT_NAME> &newPath);
};

class DesignExtractor {
 public:
    explicit DesignExtractor(std::unique_ptr<PKBWriter>);

    std::shared_ptr<ASTNode> extractProgram(std::shared_ptr<ASTNode>);

    std::unordered_map<STMT_NUM, ASSIGN_PAT> getAssignPatMap();

 private:
    ENT_SET varNameSet_;
    ENT_SET constSet_;
    ENT_SET procSet_;

    STMT_SET stmtSet_;
    STMT_SET assignSet_;
    STMT_SET printSet_;
    STMT_SET readSet_;
    STMT_SET ifSet_;
    STMT_SET whileSet_;
    STMT_SET callSet_;

    STMT_ENT_SET stmtUsePairSet_;
    STMT_ENT_SET stmtModPairSet_;
    STMT_ENT_SET stmtCallProcSet_;
    STMT_ENT_SET stmtReadVarSet_;
    STMT_ENT_SET stmtPrintVarSet_;
    STMT_ENT_SET ifCondUsePairSet_;
    STMT_ENT_SET whileCondUsePairSet_;
    STMT_ENT_SET containerCallPairSet_;

    STMT_STMT_SET stmtFollowPairSet_;
    STMT_STMT_SET stmtFollowStarPairSet_;
    STMT_STMT_SET stmtParentPairSet_;
    STMT_STMT_SET stmtParentStarPairSet_;

    ENT_ENT_SET procUsePairSet_;
    ENT_ENT_SET procModPairSet_;

    std::unordered_map<STMT_NUM, ASSIGN_PAT> assignPatMap_;
    std::unordered_map<ENT_NAME, ENT_SET> procDirectUseVarMap_;
    std::unordered_map<ENT_NAME, ENT_SET> procDirectModVarMap_;
    std::unordered_map<ENT_NAME, STMT_SET> callProcNameToStmtMap_;

    std::unique_ptr<PKBWriter> pkbWriter_;
    std::shared_ptr<ASTNode> root_;
    std::vector<STMT_NUM> containerStmtLst_;

    STMT_NUM stmtCnt_;
    ENT_NAME curProc_;

    CallGraph callGraph_;
    CFG::CFGraphBuilder CFGBuilder_;
    std::vector<CFG::CFGraph> CFGLst_;

    bool isIfCond;

    void addVarNameSetToPKB();

    void addConstantSetToPKB();

    void addProcSetToPKB();

    void addStmtUsesPairSetToPKB();

    void addStmtModifiesPairSetToPKB();

    void addStmtFollowPairSetToPKB();

    void addStmtParentPairSetToPKB();

    void addCallProcSetToPKB();

    void addReadStmtVarSetToPKB();

    void addPrintStmtVarSetToPKB();

    void addPatternsToPKB();

    void addStmtTypesToPKB();

    void addCallsToPKB();

    void addCFGToPKB();

    void addIfCondUsesPairSetToPKB();

    void addWhileCondUsesPairSetToPKB();

    void addProcUsesPairSetToPKB();

    void addProcModifiesPairSetToPKB();

    void addContainerCallPairSetToPKB();

    void analyzeProc();

    void updateStmtUsesPairSetWithContainedCalls();

    void updateStmtModsPairSetWithContainedCalls();

    void extractProc(const std::shared_ptr<ASTNode> &);

    void extractAssign(const std::shared_ptr<ASTNode> &);

    void extractRead(const std::shared_ptr<ASTNode> &);

    void extractPrint(const std::shared_ptr<ASTNode> &);

    void extractIf(const std::shared_ptr<ASTNode> &);

    void extractWhile(const std::shared_ptr<ASTNode> &);

    void extractStmtLst(const std::shared_ptr<ASTNode> &);

    void extractCall(const std::shared_ptr<ASTNode> &);

    void extractCondExpr(const std::shared_ptr<ASTNode> &);

    ENT_NAME extractLeftAssign(const std::shared_ptr<ASTNode> &);

    void extractRightAssign(const std::shared_ptr<ASTNode> &);

    void updateStmtSet();

    void updateParentsPairSet(const std::unique_ptr<std::vector<STMT_NUM>> &lst);

    void updateFollowsPairSet(const std::unique_ptr<std::vector<STMT_NUM>> &lst);

    void updateStmtUsesPairSet(STMT_NUM stmt, const ENT_NAME &varName);

    void updateStmtModsPairSet(STMT_NUM stmt, const ENT_NAME &varName);

    void updateContainerCallPairSet(const ENT_NAME &procName);

    void updateStmtUsesPairSetWithCalls();

    void updateStmtModsPairSetWithCalls();
};
