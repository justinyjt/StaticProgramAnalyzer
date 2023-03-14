#include "CFGManager.h"

#include <algorithm>
#include <cassert>
#include <utility>

namespace CFG {

using std::any_of;

void CFGManager::setGraphs(std::vector<CFGraph> &&cfg) {
    this->graphs_ = std::move(cfg);
}

int CFGManager::getIndex(STMT_NUM num) {
    int start = 0;
    int end = this->graphs_.size();
    while (start <= end) {
        int mid = start + ((end - start) >> 1);
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

    if (this->getIndex(first) != this->getIndex(second)) {
        return false;
    }

    const CFGraph &curr = getCFG(first);
    return curr.isReachable(first, second, !isTransitive);
}

bool CFGManager::isNextExists() {
    return any_of(this->graphs_.begin(), this->graphs_.end(), [](const CFGraph &curr) {
      return curr.getMaxStmtNum() - curr.getMinStmtNum() > 1;
    });
}

STMT_SET CFGManager::getConnectedStmts(STMT_NUM num, bool isAfter, bool isTransitive) {
    const CFGraph &curr = getCFG(num);
    if (isAfter) {
        return curr.getSuccessors(num, isTransitive);
    }
    return curr.getPredecessors(num, isTransitive);
}

bool CFGManager::isNextExistAfterStmtNum(STMT_NUM num, bool isTransitive) {
    return !getConnectedStmts(num, true, isTransitive).empty();
}

bool CFGManager::isNextExistBeforeStmtNum(STMT_NUM num, bool isTransitive) {
    return !getConnectedStmts(num, false, isTransitive).empty();
}

STMT_STMT_SET CFGManager::getValidNextPairs(bool isTransitive) {
    STMT_STMT_SET result;
    for (auto &curr : this->graphs_) {
        STMT_STMT_SET temp = curr.getPairwiseControlFlow(isTransitive);
        result.insert(temp.begin(), temp.end());
    }
    return result;
}
}  // namespace CFG
