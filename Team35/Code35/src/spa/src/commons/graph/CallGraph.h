#pragma once

#include "commons/graph/Graph.h"
#include "commons/types.h"

class CallGraph : public Graph<ENT_NAME> {
 public:
    CallGraph();

    void addCallRelationship(const ENT_NAME &caller, const ENT_NAME &callee);

    ENT_SET getCallees(const ENT_NAME &proc);

    ENT_ENT_SET getTransitiveCalls();

    ENT_ENT_SET getImmediateCalls();

 private:
    ENT_SET getCallEntities(Index index);
};


