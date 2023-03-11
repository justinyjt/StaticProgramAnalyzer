#pragma once

#include <functional>

#include "commons/types.h"

namespace CFG {
class CFGraphNodeData {
 public:
    explicit CFGraphNodeData(STMT_NUM stmt_num, bool is_dummy = false);
    STMT_NUM getStmtNum() const;
    bool isDummy() const;
    bool operator==(const CFGraphNodeData &other) const;
 private:
    STMT_NUM stmt_num_;
    bool is_dummy_;
};

inline CFGraphNodeData makeDummyNodeData(STMT_NUM stmt_num) {
    return CFGraphNodeData(stmt_num, true);
}

inline CFGraphNodeData makeNodeData(STMT_NUM stmt_num) {
    return CFGraphNodeData(stmt_num, false);
}
}  // namespace CFG

namespace std {
template<>
struct hash<CFG::CFGraphNodeData> {
    std::size_t operator()(CFG::CFGraphNodeData const &data) const noexcept {
        if (data.isDummy()) {
            return ~(std::hash<STMT_NUM>()(data.getStmtNum()));
        } else {
            return std::hash<STMT_NUM>()(data.getStmtNum());
        }
    }
};
}  // namespace std
