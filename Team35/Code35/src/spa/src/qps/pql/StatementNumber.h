#include "Tok.h"

class StatementNumber : public Tok {
public:
    StatementNumber(int stmtNum) : stmtNum(stmtNum), Tok(STMT_NUM) {};
    const int stmtNum;
};
