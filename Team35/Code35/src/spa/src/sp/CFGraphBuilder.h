#pragma once

#include <optional>

#include "commons/graph/CFGraph.h"

namespace CFG {
class CFGraphBuilder {
 public:
    CFGraphBuilder();
    ~CFGraphBuilder() = default;
    void addStmt(STMT_NUM stmt_num);
    void addLoop(STMT_NUM stmt_num);
    void linkToDummyNode(STMT_NUM stmt_num);
    void addDummyNode(STMT_NUM stmt_num);
    void setProcName(ENT_NAME proc_name);
    void setMaxStmtNum(STMT_NUM max_stmt_num);
    void setMinStmtNum(STMT_NUM min_stmt_num);
    CFGraph build();
    void reset();

 private:
    CFGraph cf_graph_;
    std::optional<CFGraphNodeData> last_visited_node_data_;
    ENT_NAME proc_name_;
    std::optional<STMT_NUM> max_stmt_num_;
    std::optional<STMT_NUM> min_stmt_num_;

    void setLastVisitedNode(const CFGraphNodeData &node_data);
    bool isLastVisitedNodeExist() const;

    inline void addNode(const CFGraphNodeData &node_data) {
        if (isLastVisitedNodeExist()) {
            this->cf_graph_.addEdge(this->last_visited_node_data_.value(), node_data);
        }
        setLastVisitedNode(node_data);
    }
};
}  // namespace CFG
