#ifndef TEAM35_CODE35_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_
#define TEAM35_CODE35_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_

#include <memory>
#include <string>
#include <unordered_map>
#include "pkb/PKBWriter.h"
#include "commons/ASTNode.h"

class DesignExtractor {
 public:
    explicit DesignExtractor(PKBWriter&);

    void extractProgram(std::unique_ptr<ASTNode>);

    void addVarNameSetToPKB();
    void addStmtUsesPairSetToPKB();
    void addStmtModifiesPairSetToPKB();
    void addPatternsToPKB();

    std::unordered_map<STMT_NUM, std::string> getAssignPatMap();

 private:
    ENT_SET varNameSet_;
    STMT_SET constSet_;

    STMT_ENT_SET stmtUsePairSet_;
    STMT_ENT_SET stmtModPairSet_;

    std::unordered_map<STMT_NUM, std::string> assignPatMap_;

    PKBWriter pkbWriter_;
    std::unique_ptr<ASTNode> root_;
    STMT_NUM stmtCnt_;
    std::string assignPat_;

    void extractProc(const std::unique_ptr<ASTNode>&);
    void extractAssign(const std::unique_ptr<ASTNode>&);
    std::string extractLeftAssign(const std::unique_ptr<ASTNode>&);
    std::string extractRightAssign(const std::unique_ptr<ASTNode>&);
};


#endif  // TEAM35_CODE35_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_