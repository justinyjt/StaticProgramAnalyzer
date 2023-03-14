#include "SyntaxValidator.h"

#include <cassert>
#include <utility>

SyntaxValidator::SyntaxValidator(std::unique_ptr<ILexer> lex) : scanner_(std::move(lex)) {}

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
    assert(scanner_.peek(Token::Tag::Procedure));
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

    while (!scanner_.peek(Token::Tag::RBrace)) {
        if (!validateStmt()) {
            return false;
        }
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
    assert(scanner_.isName());
    scanner_.next();

    if (!scanner_.match(Token::Tag::Assignment) || !validateExpr()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateRead() {
    assert(scanner_.peek(Token::Tag::Read));
    scanner_.next();

    if (!validateName()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validatePrint() {
    assert(scanner_.peek(Token::Tag::Print));
    scanner_.next();

    if (!validateName()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateIf() {
    assert(scanner_.peek(Token::Tag::If));
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
    assert(scanner_.peek(Token::Tag::While));
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
    assert(scanner_.peek(Token::Tag::Call));
    scanner_.next();

    if (!validateName()) {
        return false;
    }

    return scanner_.match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateExpr() {
    if (!validateTerm()) {
        return false;
    }
    if (scanner_.peek(Token::Tag::Plus)) {
        scanner_.next();
        return validateExpr();
    } else if (scanner_.peek(Token::Tag::Minus)) {
        scanner_.next();
        return validateExpr();
    } else {
        return true;
    }
}

bool SyntaxValidator::validateTerm() {
    if (!validateFactor()) {
        return false;
    }
    if (scanner_.peek(Token::Tag::Multiply)) {
        scanner_.next();
        return validateTerm();
    } else if (scanner_.peek(Token::Tag::Divide)) {
        scanner_.next();
        return validateTerm();
    } else if (scanner_.peek(Token::Tag::Modulo)) {
        scanner_.next();
        return validateTerm();
    } else {
        return true;
    }
}

bool SyntaxValidator::validateFactor() {
    if (scanner_.isName()) {
        return validateName();
    } else if (scanner_.peek(Token::Tag::Integer)) {
        return validateInt();
    } else if (scanner_.match(Token::Tag::LParen)) {
        if (!validateExpr()) {
            return false;
        }
        return scanner_.match(Token::Tag::RParen);
    } else {
        return false;
    }
}

bool SyntaxValidator::validateName() {
    if (scanner_.isName()) {
        scanner_.next();
        return true;
    }
    return false;
}

bool SyntaxValidator::validateInt() {
    return scanner_.match(Token::Tag::Integer);
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
    if (!validateExpr()) {
        return false;
    }

    if (!scanner_.match(Token::Tag::Equivalence) && !scanner_.match(Token::Tag::NotEqual)
        && !scanner_.match(Token::Tag::LessThan)
        && !scanner_.match(Token::Tag::LessThanEqualTo) && !scanner_.match(Token::Tag::GreaterThan)
        && !scanner_.match(Token::Tag::GreaterThanEqualTo)) {
        return false;
    }

    return validateExpr();
}
