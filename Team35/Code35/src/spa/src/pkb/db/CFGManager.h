#pragma once

#include <vector>

#include "commons/graph/CFGraph.h"

namespace CFG {
class CFGManager {
 public:
    CFGManager() = default;

    void setGraphs(std::vector<CFGraph> &&cfg);

    const CFGraph &getCFG(STMT_NUM num) const;

    ENT_NAME getProcName(STMT_NUM num);

    bool isValidStmtNum(STMT_NUM num) const;

    bool isNext(STMT_NUM first, STMT_NUM second, UsageType usageType) const;

    bool isNextExists() const;

    STMT_SET getConnectedStmts(STMT_NUM num, ArgType arg, UsageType usage) const;

    STMT_STMT_SET getValidNextPairs(UsageType usageType);

    STMT_SET getValidPredecessors() const;

    STMT_SET getValidSuccessors() const;

 private:
    std::vector<CFGraph> graphs_;

    int getIndex(STMT_NUM num) const;
};
}  // namespace CFG
