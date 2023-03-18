#pragma once

#include <deque>
#include <unordered_map>

#include "commons/graph/Graph.h"
#include "commons/types.h"

class CallGraph : public Graph<ENT_NAME> {
 public:
    CallGraph();

    void addCallRelationship(const ENT_NAME &caller, const ENT_NAME &callee);

    void addProcScope(ENT_NAME proc, STMT_STMT scope);

    ENT_SET getCallingProcs(const ENT_NAME &proc) const;

    ENT_ENT_SET getTransitiveCalls();

    ENT_ENT_SET getImmediateCalls();

    STMT_SET getStmts(const ENT_NAME &procName) const;

 private:
    ENT_SET getCallEntities(Index index);

    STMT_SET getStmtsByIndex(Index index) const;

    std::unordered_map<ENT_NAME, STMT_STMT> call_map_;
};


