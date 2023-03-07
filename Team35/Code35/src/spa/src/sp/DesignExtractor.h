#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "commons/ASTNode.h"
#include "CallGraph.h"
#include "pkb/PKBWriter.h"

class DesignExtractor {
 public:
    explicit DesignExtractor(std::unique_ptr<PKBWriter>);

    std::unique_ptr<ASTNode> extractProgram(std::unique_ptr<ASTNode>);

    void addVarNameSetToPKB();

    void addConstantSetToPKB();

    void addStmtUsesPairSetToPKB();

    void addStmtModifiesPairSetToPKB();

    void addStmtFollowPairSetToPKB();

    void addStmtParentPairSetToPKB();

    void addPatternsToPKB();

    void addStmtTypesToPKB();

    void addCallsToPKB();

    std::unordered_map<STMT_NUM, std::string> getAssignPatMap();

 private:
    ENT_SET varNameSet_;
    ENT_SET constSet_;
    STMT_SET stmtSet_;
    STMT_SET assignSet_;
    STMT_SET printSet_;
    STMT_SET readSet_;
    STMT_SET ifSet_;
    STMT_SET whileSet_;

    STMT_ENT_SET stmtUsePairSet_;
    STMT_ENT_SET stmtModPairSet_;
    STMT_STMT_SET stmtFollowPairSet_;
    STMT_STMT_SET stmtFollowStarPairSet_;
    STMT_STMT_SET stmtParentPairSet_;
    STMT_STMT_SET stmtParentStarPairSet_;

    std::unordered_map<STMT_NUM, std::string> assignPatMap_;

    std::unique_ptr<PKBWriter> pkbWriter_;
    std::unique_ptr<ASTNode> root_;
    std::vector<STMT_NUM> containerStmtLst_;
    STMT_NUM stmtCnt_;
    std::string assignPat_;
    ENT_NAME curProc_;
    CallGraph callGraph_;

    void extractProc(const std::unique_ptr<ASTNode> &);

    void extractAssign(const std::unique_ptr<ASTNode> &);

    void extractRead(const std::unique_ptr<ASTNode> &);

    void extractPrint(const std::unique_ptr<ASTNode> &);

    void extractIf(const std::unique_ptr<ASTNode> &);

    void extractWhile(const std::unique_ptr<ASTNode> &);

    void extractCall(const std::unique_ptr<ASTNode>&);

    void extractStmtLst(const std::unique_ptr<ASTNode> &);

    void extractCondExpr(const std::unique_ptr<ASTNode> &);

    std::string extractLeftAssign(const std::unique_ptr<ASTNode> &);

    std::string extractRightAssign(const std::unique_ptr<ASTNode> &);

    void updateStmtSet();

    void updateParentsPairSet(const std::unique_ptr<std::vector<STMT_NUM>> &lst);

    void updateFollowsPairSet(const std::unique_ptr<std::vector<STMT_NUM>> &lst);

    void updateStmtUsesPairSet(STMT_NUM stmt, std::string varName);

    void updateStmtModsPairSet(STMT_NUM stmt, std::string varName);
};
