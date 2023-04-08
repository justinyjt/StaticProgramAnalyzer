#include "CFGManager.h"

#include <algorithm>
#include <utility>

namespace CFG {

using std::any_of;

void CFGManager::setGraphs(std::vector<CFGraph> cfg) {
    this->graphs_ = std::move(cfg);
}

int CFGManager::getIndex(STMT_NUM num) const {
    int start = 0;
    int end = this->graphs_.size() - 1;
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

const CFGraph &CFGManager::getCFG(STMT_NUM num) const {
    int index = this->getIndex(num);
    return graphs_[index];
}

ENT_NAME CFGManager::getProcName(STMT_NUM num) {
    int index = this->getIndex(num);
    return graphs_[index].getProcName();
}

bool CFGManager::isValidStmtNum(STMT_NUM num) const {
    return this->getIndex(num) != -1;
}

bool CFGManager::isNext(STMT_NUM first, STMT_NUM second, UsageType usageType) const {
    if (!isValidStmtNum(first) || !isValidStmtNum(second)) {
        return false;
    }

    if (this->getIndex(first) != this->getIndex(second)) {
        return false;
    }
    if (!isValidStmtNum(first)) {
        return false;
    }
    const CFGraph &curr = getCFG(first);
    return curr.isReachable(first, second, usageType);
}

bool CFGManager::isNextExists() const {
    return any_of(this->graphs_.begin(), this->graphs_.end(), [](const CFGraph &curr) {
        return curr.getMaxStmtNum() - curr.getMinStmtNum() > 1;
    });
}

STMT_SET CFGManager::getConnectedStmts(STMT_NUM num, ArgType arg, UsageType usage) const {
    if (!isValidStmtNum(num)) {
        return {};
    }
    const CFGraph &curr = getCFG(num);
    if (arg == ArgType::Key) {
        return curr.getSuccessors(num, usage);
    }
    return curr.getPredecessors(num, usage);
}

STMT_STMT_SET CFGManager::getValidNextPairs(UsageType usageType) {
    STMT_STMT_SET result;
    for (auto &curr : this->graphs_) {
        const STMT_STMT_SET &temp = curr.getPairwiseControlFlow(usageType);
        result.insert(temp.begin(), temp.end());
    }
    return result;
}

STMT_SET CFGManager::getValidPredecessors() const {
    STMT_SET result;
    for (auto &curr : this->graphs_) {
        STMT_SET temp = curr.getAllPredecessors();
        result.insert(temp.begin(), temp.end());
    }
    return result;
}

STMT_SET CFGManager::getValidSuccessors() const {
    STMT_SET result;
    for (auto &curr : this->graphs_) {
        STMT_SET temp = curr.getAllSuccessors();
        result.insert(temp.begin(), temp.end());
    }
    return result;
}
}  // namespace CFG
