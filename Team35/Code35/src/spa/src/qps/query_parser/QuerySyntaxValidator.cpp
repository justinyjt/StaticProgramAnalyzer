#include "QuerySyntaxValidator.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/expr_validator/ExprValidator.h"

#include <cassert>
#include <utility>

QuerySyntaxValidator::QuerySyntaxValidator(std::unique_ptr<ILexer> lex) : scanner_(std::move(lex)) {}

bool QuerySyntaxValidator::validateQuery() {
    // validate declarationLst
    if (!validateDeclarationLst()) {
        return false;
    }

    // validate Select cl
    if (!validateResultClause()) {
        return false;
    }

    // validate clauses while pattern
    if (!validateClauseLst()) {
        return false;
    }
    return true;
}

bool QuerySyntaxValidator::validateDeclarationLst() {
    // validate design-entity
    while (scanner_.peekDesignEntity()) {
        if (!validateDeclaration()) {
            return false;
        }
    }
    return true;
}


bool QuerySyntaxValidator::validateDeclaration() {
    assert(scanner_.peekDesignEntity());
    scanner_.next();
    if (!scanner_.isName() && !scanner_.peek(Token::Tag::Bool)) {
        return false;
    }
    scanner_.next();
    while (scanner_.peek(Token::Tag::Comma)) {
        scanner_.next();
        if (scanner_.isName()) {
            scanner_.next();
        } else {
            return false;
        }
    }
    if (!scanner_.peek(Token::Tag::SemiColon)) {
        return false;
    }
    scanner_.next();
    return true;
}

bool QuerySyntaxValidator::validateResultClause() {
    if (!scanner_.peek(Token::Tag::Select)) {
        return false;
    }
    scanner_.next();

    if (scanner_.peek(Token::Tag::Bool)) {
        scanner_.next();
        return true;
    }

    if (validateTuple()) {
        return true;
    }

    return false;
}

bool QuerySyntaxValidator::validateClauseLst() {
    while (!scanner_.peek(Token::Tag::EndOfFile)) {
        if (!validateClause()) {
            return false;
        }
    }
    return true;
}

bool QuerySyntaxValidator::validateClause() {
    // check such that
    if (scanner_.peekSuchThatClause()) {
        return validateSuchThatClause();
    } else if (scanner_.peek(Token::Tag::Pattern)) {
        return validatePatternClause();
    } else if (scanner_.peek(Token::Tag::With)) {
        return validateWithClause();
    } else {
        return false;
    }
}

bool QuerySyntaxValidator::validateSuchThatClause() {
    assert(scanner_.peek(Token::Tag::Such));
    scanner_.next();
    assert(scanner_.peek(Token::Tag::That));
    scanner_.next();
    return validateRelCond();
}

bool QuerySyntaxValidator::validateRelCond() {
    if (!validateRelRef()) {
        return false;
    }
    while (scanner_.peek(Token::Tag::And)) {
        scanner_.next();
        if (!validateRelRef()) {
            return false;
        }
    }
    return true;
}

bool QuerySyntaxValidator::validateRelRef() {
    if (scanner_.peek(Token::Tag::Follows) || scanner_.peek(Token::Tag::Parent)) {
        scanner_.next();
        if (scanner_.peek(Token::Tag::Star)) {
            scanner_.next();
        }
        return validateStmtStmtRef();
    } else if (scanner_.peek(Token::Tag::Modifies) || scanner_.peek(Token::Tag::Uses)) {
        scanner_.next();
        return validateModifiesUsesRef();
    } else if (scanner_.peek(Token::Tag::Calls)) {
        scanner_.next();
        if (scanner_.peek(Token::Tag::Star)) {
            scanner_.next();
        }
        return validateEntEntRef();
    } else {
        return false;
    }
}

bool QuerySyntaxValidator::validateStmtStmtRef() {
    if (!scanner_.peek(Token::Tag::LParen)) {
        return false;
    }
    scanner_.next();
    if (!validateStmtRef()) {
        return false;
    }
    if (!scanner_.peek(Token::Tag::Comma)) {
        return false;
    }
    scanner_.next();
    if (!validateStmtRef()) {
        return false;
    }

    if (!scanner_.peek(Token::Tag::RParen)) {
        return false;
    }
    scanner_.next();

    return true;
}

bool QuerySyntaxValidator::validateEntEntRef() {
    if (!scanner_.peek(Token::Tag::LParen)) {
        return false;
    }
    scanner_.next();
    if (!validateEntRef()) {
        return false;
    }
    if (!scanner_.peek(Token::Tag::Comma)) {
        return false;
    }
    scanner_.next();
    if (!validateEntRef()) {
        return false;
    }
    if (!scanner_.peek(Token::Tag::RParen)) {
        return false;
    }
    scanner_.next();

    return true;
}

bool QuerySyntaxValidator::validateModifiesUsesRef() {
    if (!scanner_.peek(Token::Tag::LParen)) {
        return false;
    }
    scanner_.next();

    scanner_.saveState();
    if (!validateEntRef()) {
        scanner_.restoreState();
        if (!validateStmtRef()) {
            return false;
        }
    }

    if (!scanner_.peek(Token::Tag::Comma)) {
        return false;
    }
    scanner_.next();

    if (!validateEntRef()) {
        return false;
    }

    if (!scanner_.peek(Token::Tag::RParen)) {
        return false;
    }
    scanner_.next();
    return true;
}

bool QuerySyntaxValidator::validatePatternClause() {
    assert(scanner_.peek(Token::Tag::Pattern));
    scanner_.next();
    return validatePatternCond();
}

bool QuerySyntaxValidator::validatePatternCond() {
    if (!validatePattern()) {
        return false;
    }
    while (scanner_.peek(Token::Tag::And)) {
        scanner_.next();
        if (!validatePattern()) {
            return false;
        }
    }
    return true;
}

bool QuerySyntaxValidator::validatePattern() {
    scanner_.saveState();
    if (validateAssignPattern()) {
        return true;
    }
    scanner_.restoreState();
    if (validateWhilePattern()) {
        return true;
    }
    scanner_.restoreState();
    if (validateIfPattern()) {
        return true;
    }
    scanner_.restoreState();
    return false;
}

bool QuerySyntaxValidator::validateAssignPattern() {
    if (!scanner_.isName()) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::LParen)) {
        return false;
    }
    scanner_.next();
    if (!validateEntRef()) {
        return false;
    }
    if (!scanner_.peek(Token::Tag::Comma)) {
        return false;
    }
    scanner_.next();
    if (!validateExpressionSpec()) {
        return false;
    }
    if (!scanner_.peek(Token::Tag::RParen)) {
        return false;
    }
    scanner_.next();

    return true;
}

bool QuerySyntaxValidator::validateWhilePattern() {
    if (!scanner_.isName()) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::LParen)) {
        return false;
    }
    scanner_.next();
    if (!validateEntRef()) {
        return false;
    }
    if (!scanner_.peek(Token::Tag::Comma)) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::Underscore)) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::RParen)) {
        return false;
    }
    scanner_.next();

    return true;
}

bool QuerySyntaxValidator::validateIfPattern() {
    if (!scanner_.isName()) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::LParen)) {
        return false;
    }
    scanner_.next();
    if (!validateEntRef()) {
        return false;
    }
    if (!scanner_.peek(Token::Tag::Comma)) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::Underscore)) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::Comma)) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::Underscore)) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::RParen)) {
        return false;
    }
    scanner_.next();

    return true;
}

bool QuerySyntaxValidator::validateWithClause() {
    assert(scanner_.peek(Token::Tag::With));
    scanner_.next();
    return validateAttrCond();
}

bool QuerySyntaxValidator::validateAttrCond() {
    if (!validateAttrCompare()) {
        return false;
    }
    while (scanner_.peek(Token::Tag::And)) {
        scanner_.next();
        if (!validateAttrCompare()) {
            return false;
        }
    }
    return true;
}

bool QuerySyntaxValidator::validateAttrCompare() {
    if (!validateRef()) {
        return false;
    }
    if (!scanner_.peek(Token::Tag::Equal)) {
        return false;
    }
    scanner_.next();
    if (!validateRef()) {
        return false;
    }
    return true;
}

bool QuerySyntaxValidator::validateRef() {
    if (scanner_.peekIdent() || scanner_.peek(Token::Tag::Integer)) {
        scanner_.next();
        return true;
    } else {
        return validateAttrRef();
    }
}

bool QuerySyntaxValidator::validateAttrRef() {
    if (!scanner_.isName()) {
        return false;
    }
    scanner_.next();
    if (!scanner_.peek(Token::Tag::Dot)) {
        return false;
    }
    scanner_.next();
    if (validateAttrName()) {
        return true;
    }
    return false;
}

bool QuerySyntaxValidator::validateAttrName() {
    if (scanner_.peekLexeme() == "procName" || scanner_.peekLexeme() == "varName" || scanner_.peekLexeme() == "value") {
        scanner_.next();
        return true;
    } else if (scanner_.peekLexeme() == "stmt") {
        scanner_.next();
        if (!scanner_.peek(Token::Tag::Hex)) {
            return false;
        }
        scanner_.next();
        return true;
    } else {
        return false;
    }
}

bool QuerySyntaxValidator::validateStmtRef() {
    if (scanner_.isName() || scanner_.peek(Token::Tag::Underscore) || scanner_.peek(Token::Tag::Integer)) {
        scanner_.next();
        return true;
    }
    return false;
}

bool QuerySyntaxValidator::validateEntRef() {
    if (scanner_.isName() || scanner_.peek(Token::Tag::Underscore) || scanner_.peekIdent()) {
        scanner_.next();
        return true;
    }
    return false;
}

bool QuerySyntaxValidator::validateExpressionSpec() {
    if (scanner_.peek(Token::Tag::String)) {
        std::string expr = scanner_.peekLexeme();
        scanner_.next();
        std::unique_ptr<ILexer> lxr =
                LexerFactory::createLexer(expr, LexerFactory::LexerType::Expression);
        TokenScanner scanner(std::move(lxr));
        ExprValidator exprValidator(scanner);
        if (!exprValidator.validateExpr()) {
            return false;
        }
        return true;
    } else if (scanner_.peek(Token::Tag::Underscore)) {
        scanner_.next();
        if (scanner_.peek(Token::Tag::String) && scanner_.peekOffset(Token::Tag::Underscore, 1)) {
            std::string expr = scanner_.peekLexeme();
            std::unique_ptr<ILexer> lxr =
                    LexerFactory::createLexer(expr, LexerFactory::LexerType::Expression);
            TokenScanner scanner(std::move(lxr));
            ExprValidator exprValidator(scanner);
            if (!exprValidator.validateExpr()) {
                return false;
            }
            scanner_.next();
            scanner_.next();
        } else {
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool QuerySyntaxValidator::validateTuple() {
    if (validateElem()) {
        return true;
    } else if (scanner_.peek(Token::Tag::LessThan)) {
        return validateMultipleElem();
    }
    return false;
}

bool QuerySyntaxValidator::validateMultipleElem() {
    assert(scanner_.peek(Token::Tag::LessThan));
    scanner_.next();
    if (!validateElem()) {
        return false;
    }

    while (scanner_.peek(Token::Tag::Comma)) {
        scanner_.next();
        if (!validateElem()) {
            return false;
        }
    }

    if (!scanner_.peek(Token::Tag::GreaterThan)) {
        return false;
    }
    scanner_.next();
    return true;
}

bool QuerySyntaxValidator::validateElem() {
    scanner_.saveState();
    if (validateAttrRef()) {
        return true;
    }

    scanner_.restoreState();
    if (scanner_.isName()) {
        scanner_.next();
        return true;
    } else {
        return false;
    }
}

std::deque<std::unique_ptr<Token>> QuerySyntaxValidator::getTokenLst() {
    return scanner_.getTokenLst();
}
