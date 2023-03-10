#include "CFGManager.h"

#include <cassert>

namespace CFG {
CFGManager::CFGManager() {}

void CFGManager::setGraphs(std::vector<CFGraph> &&cfg) {
    this->graphs_ = cfg;
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
}  // namespace CFG
