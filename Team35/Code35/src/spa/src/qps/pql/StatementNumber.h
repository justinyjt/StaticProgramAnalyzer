#include "Tok.h"
#include "Arg.h"

class StatementNumber : public Tok, public StmtRef {
public:
    StatementNumber(int stmtNum) : stmtNum(stmtNum) {};
    const int stmtNum;
};
