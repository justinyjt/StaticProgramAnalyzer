#include "CFGManager.h"

#include <cassert>
#include <utility>

namespace CFG {
CFGManager::CFGManager() {}

void CFGManager::setGraphs(std::vector<CFGraph> &&cfg) {
    this->graphs_ = std::move(cfg);
}

int CFGManager::getIndex(STMT_NUM num) {
    int start = 0;
    int end = this->graphs_.size();
    while (start <= end) {
        uint32_t mid = start + (end - start) / 2;
        if (this->graphs_[mid].getMaxStmtNum() >= num && this->graphs_[mid].getMinStmtNum() <= num) {
            return mid;
        } else if (this->graphs_[mid].getMaxStmtNum() < num) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return -1;
}

const CFGraph &CFGManager::getCFG(STMT_NUM num) {
    int index = this->getIndex(num);
    assert(index != -1);
    return graphs_[index];
}

ENT_NAME CFGManager::getProcName(STMT_NUM num) {
    int index = this->getIndex(num);
    assert(index != -1);
    return graphs_[index].getProcName();
}

bool CFGManager::isValidStmtNum(STMT_NUM num) {
    return this->getIndex(num) != -1;
}

bool CFGManager::isNext(STMT_NUM first, STMT_NUM second, bool isTransitive) {
    if (!isValidStmtNum(first) || !isValidStmtNum(second)) {
        return false;
    }
    const CFGraph &curr = getCFG(first);
    if (isTransitive) {
        return curr.isReachable(first, second);
    }
    STMT_SET successors = curr.getSuccessors(first, isTransitive);
    for (auto successor : successors) {
        if (second == successor) {
            return true;
        }
    }
    return false;
}

bool CFGManager::isNextExists() {
    for (auto &curr : this->graphs_) {
        if (curr.getMaxStmtNum() - curr.getMinStmtNum() > 1) {
            return true;
        }
    }
    return false;
}

STMT_SET CFGManager::getConnectedStmts(STMT_NUM num, bool isFirstArg, bool isTransitive) {
    const CFGraph &curr = getCFG(num);
    if (isFirstArg) {
        return curr.getSuccessors(num, isTransitive);
    }
    return curr.getPredecessors(num, isTransitive);
}

bool CFGManager::isNextExistByKey(STMT_NUM num, bool isTransitive) {
    return !getConnectedStmts(num, true, isTransitive).empty();
}

bool CFGManager::isNextExistByVal(STMT_NUM num, bool isTransitive) {
    return !getConnectedStmts(num, false, isTransitive).empty();
}

STMT_STMT_SET CFGManager::getValidNextPairs(bool isTransitive) {
    STMT_STMT_SET result;
    for (auto &curr : this->graphs_) {
        STMT_STMT_SET temp = curr.getPairwiseControlFlow(isTransitive);
        result.insert(temp.begin(), temp.end());
    }
}
}  // namespace CFG
