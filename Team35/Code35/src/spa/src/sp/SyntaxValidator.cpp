#include "SyntaxValidator.h"

#include <utility>

#include "commons/expr_validator/ExprValidator.h"

SyntaxValidator::SyntaxValidator(std::unique_ptr<ILexer> lex) : scanner_(std::move(lex)), expr_validator_(scanner_) {}

void SyntaxValidator::reset() {
    scanner_.reset();
}

bool SyntaxValidator::validateProgram() {
    reset();
    while (scanner_.peek(Token::Tag::Procedure)) {
        if (!validateProc()) {
            return false;
        }
    }
    return scanner_.match(Token::Tag::EndOfFile);
}

bool SyntaxValidator::validateProc() {
    scanner_.next();
    if (!scanner_.isName()) {
        return false;
    }
    scanner_.next();
    return validateStmtLst();
}

bool SyntaxValidator::validateStmtLst() {
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

bool SyntaxValidator::validateStmt() {
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

bool SyntaxValidator::validateAssign() {
    scanner_.next();

    if (!scanner_.match(Token::Tag::Assignment) || !expr_validator_.validateExpr()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateRead() {
    scanner_.next();

    if (!validateName()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validatePrint() {
    scanner_.next();

    if (!validateName()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateIf() {
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

bool SyntaxValidator::validateWhile() {
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

bool SyntaxValidator::validateCall() {
    scanner_.next();

    if (!validateName()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateName() {
    if (scanner_.isName()) {
        scanner_.next();
        return true;
    }
    return false;
}

std::deque<std::unique_ptr<Token>> SyntaxValidator::getTokenLst() {
    return scanner_.getTokenLst();
}

bool SyntaxValidator::validateCondExpr() {
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

bool SyntaxValidator::validateRelExpr() {
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
