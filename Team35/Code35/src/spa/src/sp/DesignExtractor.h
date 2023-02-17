#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "commons/ASTNode.h"
#include "pkb/PKBWriter.h"

class DesignExtractor {
 public:
    explicit DesignExtractor(std::unique_ptr<PKBWriter>);

    std::unique_ptr<ASTNode> extractProgram(std::unique_ptr<ASTNode>);

    void addVarNameSetToPKB();
    void addStmtUsesPairSetToPKB();
    void addStmtModifiesPairSetToPKB();
    void addStmtFollowPairSetToPKB();
    void addPatternsToPKB();
    void addStmtTypesToPKB();

    std::unordered_map<STMT_NUM, std::string> getAssignPatMap();

 private:
    ENT_SET varNameSet_;
    STMT_SET constSet_;
    STMT_SET stmtSet_;
    STMT_SET assignSet_;
    STMT_SET printSet_;
    STMT_SET readSet_;

    STMT_ENT_SET stmtUsePairSet_;
    STMT_ENT_SET stmtModPairSet_;
    STMT_STMT_SET stmtFollowPairSet_;
    STMT_STMT_SET stmtFollowStarPairSet_;

    std::unordered_map<STMT_NUM, std::string> assignPatMap_;

    std::unique_ptr<PKBWriter> pkbWriter_;
    std::unique_ptr<ASTNode> root_;
    STMT_NUM stmtCnt_;
    std::string assignPat_;

    void extractProc(const std::unique_ptr<ASTNode> &);
    void extractAssign(const std::unique_ptr<ASTNode> &);
    void extractRead(const std::unique_ptr<ASTNode> &);
    void extractPrint(const std::unique_ptr<ASTNode> &);
    void extractStmtLst(const std::unique_ptr<ASTNode> &);

    std::string extractLeftAssign(const std::unique_ptr<ASTNode> &);
    std::string extractRightAssign(const std::unique_ptr<ASTNode> &);

    void updateStmtSet();
    void updateFollowsPairSet(STMT_NUM start_stmt, STMT_NUM end_stmt);
};
