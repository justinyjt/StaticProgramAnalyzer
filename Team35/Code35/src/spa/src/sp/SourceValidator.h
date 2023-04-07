#pragma once

#include <memory>
#include <deque>

#include "commons/lexer/ILexer.h"
#include "commons/token/Token.h"
#include "commons/token_scanner/SimpleTokenScanner.h"
#include "commons/expr_validator/ExprValidator.h"
#include "commons/graph/CallGraph.h"
#include "commons/types.h"

class SourceValidator {
 public:
    explicit SourceValidator(SimpleTokenScanner &scanner);
    bool validate();

 private:
    SimpleTokenScanner &scanner_;
    ExprValidator expr_validator_;
    CallGraph call_graph_;
    ENT_NAME current_proc_;

    void reset();

    bool validateProc();
    bool validateStmtLst();
    bool validateStmt();

    bool validateAssign();
    bool validateRead();
    bool validatePrint();
    bool validateIf();
    bool validateWhile();
    bool validateCall();

    bool validateCondExpr();
    bool validateRelExpr();
    bool validateName();
};
