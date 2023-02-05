#include "qps/result/Result.h"
#include "qps/clause/Arg.h"
#include "pkb/PKBReader.h"
#include "commons/types.h"

/* 
Base class for select clause in PQL (suchthat-cl, pattern-cl)
*/
class Clause {
  public:
    Arg& first;
    Arg& second;
    static PKBReader db;

    Clause(Arg& first, Arg& second);

    virtual Result evaluate();
};
