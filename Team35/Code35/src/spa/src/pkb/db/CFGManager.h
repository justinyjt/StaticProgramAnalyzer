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
};
}  // namespace CFG
