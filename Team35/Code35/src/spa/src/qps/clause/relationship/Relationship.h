#include "qps/clause/Clause.h"
#include "qps/clause/Arg.h"
#include "commons/types.h"

/*
Base class for any Relationship:
Follows | FollowsT | Parent | ParentT | UsesS | UsesP | ModifiesS | ModifiesP
*/
class Relationship : public Clause {
    public:
        Relationship(Arg&, Arg&);
};
