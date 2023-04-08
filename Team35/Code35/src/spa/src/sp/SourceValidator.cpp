#include "SourceValidator.h"

#include "commons/expr_validator/ExprValidator.h"

SourceValidator::SourceValidator(SimpleTokenScanner &scanner)
        : scanner_(scanner), expr_validator_(scanner_), call_graph_(), current_proc_() {}

void SourceValidator::reset() {
    scanner_.reset();
    call_graph_.reset();
}

bool SourceValidator::validate() {
    reset();
    while (scanner_.peek(Token::Tag::Procedure)) {
        if (!validateProc()) {
            return false;
        }
    }
    if (call_graph_.isCyclic()) {
        return false;
    }
    return scanner_.match(Token::Tag::EndOfFile);
}

bool SourceValidator::validateProc() {
    scanner_.next();
    ENT_NAME proc_name = scanner_.peekLexeme();
    if (!validateName()) {
        return false;
    }
    current_proc_ = proc_name;
    return validateStmtLst();
}

bool SourceValidator::validateStmtLst() {
    if (!scanner_.match(Token::Tag::LBrace)) {
        return false;
    }

    int cnt = 0;
    while (!scanner_.peek(Token::Tag::RBrace)) {
        cnt++;
        if (!validateStmt()) {
            return false;
        }
    }
    if (cnt == 0) {
        return false;
    }

    return scanner_.match(Token::Tag::RBrace);
}

bool SourceValidator::validateStmt() {
    if (scanner_.isAssign()) {
        return validateAssign();
    } else if (scanner_.peek(Token::Tag::Read)) {
        return validateRead();
    } else if (scanner_.peek(Token::Tag::Print)) {
        return validatePrint();
    } else if (scanner_.peek(Token::Tag::If)) {
        return validateIf();
    } else if (scanner_.peek(Token::Tag::While)) {
        return validateWhile();
    } else if (scanner_.peek(Token::Tag::Call)) {
        return validateCall();
    } else {
        return false;
    }
}

bool SourceValidator::validateAssign() {
    scanner_.next();

    if (!scanner_.match(Token::Tag::Assignment) || !expr_validator_.validateExpr()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SourceValidator::validateRead() {
    scanner_.next();

    if (!validateName()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SourceValidator::validatePrint() {
    scanner_.next();

    if (!validateName()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SourceValidator::validateIf() {
    scanner_.next();

    if (!scanner_.match(Token::Tag::LParen)) {
        return false;
    }

    if (!validateCondExpr()) {
        return false;
    }

    if (!scanner_.match(Token::Tag::RParen)) {
        return false;
    }

    if (!scanner_.match(Token::Tag::Then)) {
        return false;
    }

    if (!validateStmtLst()) {
        return false;
    }

    if (!scanner_.match(Token::Tag::Else)) {
        return false;
    }

    return validateStmtLst();
}

bool SourceValidator::validateWhile() {
    scanner_.next();

    if (!scanner_.match(Token::Tag::LParen)) {
        return false;
    }

    if (!validateCondExpr()) {
        return false;
    }

    if (!scanner_.match(Token::Tag::RParen)) {
        return false;
    }

    return validateStmtLst();
}

bool SourceValidator::validateCall() {
    scanner_.next();
    ENT_NAME proc_name = scanner_.peekLexeme();
    if (!validateName()) {
        return false;
    }
    call_graph_.addCallRelationship(current_proc_, proc_name);
    return scanner_.match(Token::Tag::SemiColon);
}

bool SourceValidator::validateName() {
    if (scanner_.isName()) {
        scanner_.next();
        return true;
    }
    return false;
}

bool SourceValidator::validateCondExpr() {
    if (scanner_.peek(Token::Tag::LogicalNot)) {
        scanner_.next();
        if (!scanner_.match(Token::Tag::LParen)) {
            return false;
        }

        if (!validateCondExpr()) {
            return false;
        }

        return scanner_.match(Token::Tag::RParen);
    } else if (scanner_.peek(Token::Tag::LParen)) {
        if (!scanner_.isCondExprSeparatedByLogicalOperator()) {
            return validateRelExpr();
        }

        // && || exist
        if (!scanner_.match(Token::Tag::LParen)) {
            return false;
        }

        if (!validateCondExpr()) {
            return false;
        }

        if (!scanner_.match(Token::Tag::RParen)) {
            return false;
        }

        scanner_.next();

        if (!scanner_.match(Token::Tag::LParen)) {
            return false;
        }

        if (!validateCondExpr()) {
            return false;
        }

        return scanner_.match(Token::Tag::RParen);
    } else {
        return validateRelExpr();
    }
}

bool SourceValidator::validateRelExpr() {
    if (!expr_validator_.validateExpr()) {
        return false;
    }

    if (!scanner_.match(Token::Tag::Equivalence) && !scanner_.match(Token::Tag::NotEqual)
            && !scanner_.match(Token::Tag::LessThan)
            && !scanner_.match(Token::Tag::LessThanEqualTo) && !scanner_.match(Token::Tag::GreaterThan)
            && !scanner_.match(Token::Tag::GreaterThanEqualTo)) {
        return false;
    }

    return expr_validator_.validateExpr();
}
