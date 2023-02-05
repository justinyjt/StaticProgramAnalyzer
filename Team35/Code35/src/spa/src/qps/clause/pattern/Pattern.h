#include "qps/clause/Clause.h"
#include "qps/clause/Arg.h"
#include "qps/result/Result.h"

class Pattern : public Clause {
    public:
        Pattern(EntRef& first, ExpressionSpec& second);
        Result evaluate(PKBReader*);
};
