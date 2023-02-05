#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H
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
#endif //SPA_DESIGNEXTRACTOR_H
