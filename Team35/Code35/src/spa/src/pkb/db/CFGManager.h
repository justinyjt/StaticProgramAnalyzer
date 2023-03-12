#pragma once

#include <vector>

#include "commons/graph/CFGraph.h"

namespace CFG {

class CFGManager {
 private:
    std::vector<CFGraph> graphs_;

    int getIndex(STMT_NUM num);

 public:
    CFGManager();

    void setGraphs(std::vector<CFGraph> &&cfg);

    const CFGraph &getCFG(STMT_NUM num);

    ENT_NAME getProcName(STMT_NUM num);

    bool isValidStmtNum(STMT_NUM num);

    bool isNext(STMT_NUM first, STMT_NUM second, bool isTransitive);

    bool isNextExists();

    STMT_SET getConnectedStmts(STMT_NUM num, bool isFirstArg, bool isTransitive);

    bool isNextExistByKey(STMT_NUM num, bool isTransitive);

    bool isNextExistByVal(STMT_NUM num, bool isTransitive);

    STMT_STMT_SET getValidNextPairs(bool isTransitive);
};
}  // namespace CFG
