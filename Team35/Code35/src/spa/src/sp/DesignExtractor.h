#ifndef TEAM35_CODE35_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_
#define TEAM35_CODE35_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_

#include <memory>
#include <string>
#include <unordered_map>
#include "pkb/PKB.h"
#include "commons/ASTNode.h"

class DesignExtractor {
 public:
    explicit DesignExtractor(PKB&, std::unique_ptr<ASTNode>);

    void extractProgram();

    void addVarNameSetToPKB();
    void addConstSetToPKB();
    void addStmtUsesPairSetToPKB();
    void addStmtModifiesPairSetToPKB();
    void addPatternsToPKB();

    std::unordered_map<STMT_NUM, std::string> getAssignPatMap();

 private:
    NAME_SET varNameSet_;
    INTEGER_SET constSet_;

    STMT_NAME_PAIR_SET stmtUsePairSet_;
    STMT_NAME_PAIR_SET stmtModPairSet_;

    std::unordered_map<STMT_NUM, std::string> assignPatMap_;

    PKB pkb_;
    std::unique_ptr<ASTNode> root_;
    STMT_NUM stmtCnt_;
    std::string assignPat_;

    void extractProc(const std::unique_ptr<ASTNode>&);
    void extractAssign(const std::unique_ptr<ASTNode>&);
    std::string extractLeftAssign(const std::unique_ptr<ASTNode>&);
    std::string extractRightAssign(const std::unique_ptr<ASTNode>&);
};


#endif  // TEAM35_CODE35_SRC_SPA_SRC_SP_DESIGNEXTRACTOR_H_
