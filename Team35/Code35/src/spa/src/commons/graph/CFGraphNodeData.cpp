#include "CFGraphNodeData.h"

namespace CFG {
CFGraphNodeData::CFGraphNodeData(STMT_NUM stmt_num, bool is_dummy) : stmt_num_(stmt_num), is_dummy_(is_dummy) {}

STMT_NUM CFGraphNodeData::getStmtNum() const {
    return stmt_num_;
}

bool CFGraphNodeData::isDummy() const {
    return is_dummy_;
}

bool CFGraphNodeData::operator==(const CFGraphNodeData &other) const {
    return is_dummy_ == other.is_dummy_ && stmt_num_ == other.stmt_num_;
}
}  // namespace CFG
