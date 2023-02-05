#include "Relationship.h"
#include "qps/clause/Arg.h"
#include "qps/result/result.h"

class Modify : public Relationship {
public:
    Modify(StmtRef& first, EntRef& second);
    Result evaluate();
};

class ModifyS : public Relationship {
public:
    ModifyS(EntRef& first, EntRef& second);
};
