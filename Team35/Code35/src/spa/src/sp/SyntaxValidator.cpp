#include "SyntaxValidator.h"

#include <cassert>
#include <utility>

SyntaxValidator::SyntaxValidator(std::unique_ptr<ILexer> lex) : lex_(std::move(lex)), token_lst_() {}

void SyntaxValidator::initialise() {
    cur_idx_ = 0;
    lex_->reset();
    token_lst_.clear();
    std::unique_ptr<Token> cur = lex_->scan();
    while (cur->getTag() != Token::Tag::EndOfFile) {
        token_lst_.push_back(std::move(cur));
        cur = lex_->scan();
    }
    token_lst_.push_back(std::move(cur));
}

bool SyntaxValidator::validateProgram() {
    initialise();
    while (peek(Token::Tag::Procedure)) {
        if (!validateProc()) {
            return false;
        }
    }
    return match(Token::Tag::EndOfFile);
}

bool SyntaxValidator::validateProc() {
    assert(peek(Token::Tag::Procedure));
    next();
    if (!isName()) {
        return false;
    }
    next();
    return validateStmtLst();
}

bool SyntaxValidator::validateStmtLst() {
    if (!match(Token::Tag::LBrace)) {
        return false;
    }

    while (!peek(Token::Tag::RBrace)) {
        if (!validateStmt()) {
            return false;
        }
    }

    return match(Token::Tag::RBrace);
}

bool SyntaxValidator::validateStmt() {
    if (peekOffset(Token::Tag::Assignment, 1) && isName()) {
        return validateAssign();
    } else if (peek(Token::Tag::Read)) {
        return validateRead();
    } else if (peek(Token::Tag::Print)) {
        return validatePrint();
    } else if (peek(Token::Tag::If)) {
        return validateIf();
    } else if (peek(Token::Tag::While)) {
        return validateWhile();
    } else {
        return false;
    }
}

bool SyntaxValidator::validateAssign() {
    assert(peek(Token::Tag::Name));
    next();

    if (!match(Token::Tag::Assignment) || !validateExpr()) {
        return false;
    }

    return match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateRead() {
    assert(peek(Token::Tag::Read));
    next();

    if (!validateName()) {
        return false;
    }

    return match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validatePrint() {
    assert(peek(Token::Tag::Print));
    next();

    if (!validateName()) {
        return false;
    }

    return match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateIf() {
    assert(peek(Token::Tag::If));
    next();

    if (!match(Token::Tag::LParen)) {
        return false;
    }

    if (!validateCondExpr()) {
        return false;
    }

    if (!match(Token::Tag::RParen)) {
        return false;
    }

    if (!match(Token::Tag::Then)) {
        return false;
    }

    if (!validateStmtLst()) {
        return false;
    }

    if (!match(Token::Tag::Else)) {
        return false;
    }

    return validateStmtLst();
}

bool SyntaxValidator::validateWhile() {
    assert(peek(Token::Tag::While));
    next();

    if (!match(Token::Tag::LParen)) {
        return false;
    }

    if (!validateCondExpr()) {
        return false;
    }

    if (!match(Token::Tag::RParen)) {
        return false;
    }

    return validateStmtLst();
}

bool SyntaxValidator::validateExpr() {
    if (!validateTerm()) {
        return false;
    }
    if (peek(Token::Tag::Plus)) {
        next();
        return validateExpr();
    } else if (peek(Token::Tag::Minus)) {
        next();
        return validateExpr();
    } else {
        return true;
    }
}

bool SyntaxValidator::validateTerm() {
    if (!validateFactor()) {
        return false;
    }
    if (peek(Token::Tag::Multiply)) {
        next();
        return validateTerm();
    } else if (peek(Token::Tag::Divide)) {
        next();
        return validateTerm();
    } else if (peek(Token::Tag::Modulo)) {
        next();
        return validateTerm();
    } else {
        return true;
    }
}

bool SyntaxValidator::validateFactor() {
    if (isName()) {
        return validateName();
    } else if (peek(Token::Tag::Integer)) {
        return validateInt();
    } else if (match(Token::Tag::LParen)) {
        if (!validateExpr()) {
            return false;
        }
        return match(Token::Tag::RParen);
    } else {
        return false;
    }
}

bool SyntaxValidator::validateName() {
    if (isName()) {
        next();
        return true;
    }
    return false;
}

bool SyntaxValidator::isName() {
    return peek(Token::Tag::Name) || peek(Token::Tag::Call) || peek(Token::Tag::Read) || peek(Token::Tag::Print)
        || peek(Token::Tag::Procedure) || peek(Token::Tag::Else) || peek(Token::Tag::Then) || peek(Token::Tag::If)
        || peek(Token::Tag::While);
}

bool SyntaxValidator::validateInt() {
    return match(Token::Tag::Integer);
}

bool SyntaxValidator::peek(Token::Tag tag) {
    return (token_lst_[cur_idx_]->getTag() == tag);
}

bool SyntaxValidator::match(Token::Tag tag) {
    if (token_lst_[cur_idx_]->getTag() == tag) {
        if (tag != Token::Tag::EndOfFile) {
            next();
        }
        return true;
    }
    return false;
}

bool SyntaxValidator::peekOffset(Token::Tag tag, uint32_t offset) {
    if (!isOffsetValid(offset)) {
        return false;
    }
    return (token_lst_[cur_idx_ + offset]->getTag() == tag);
}

bool SyntaxValidator::isOffsetValid(uint32_t offset) {
    return (cur_idx_ + offset < token_lst_.size());
}

void SyntaxValidator::next() {
    ++cur_idx_;
}

std::deque<std::unique_ptr<Token>> SyntaxValidator::getTokenLst() {
    std::reverse(token_lst_.begin(), token_lst_.end());
    return std::move(this->token_lst_);
}

bool SyntaxValidator::isCondExprSeparatedByLogicalOperator() {
    uint32_t offset = 1;
    int unmatched_paren = 1;
    while (isOffsetValid(offset) && unmatched_paren > 0) {
        if (peekOffset(Token::Tag::LParen, offset)) {
            ++unmatched_paren;
        } else if (peekOffset(Token::Tag::RParen, offset)) {
            --unmatched_paren;
        }
        ++offset;
    }
    if (!isOffsetValid(offset)) {
        return false;
    }
    bool is_logical_operator_exist =
        peekOffset(Token::Tag::LogicalAnd, offset) || peekOffset(Token::Tag::LogicalOr, offset);
    return is_logical_operator_exist;
}

bool SyntaxValidator::validateCondExpr() {
    if (peek(Token::Tag::LogicalNot)) {
        next();
        if (!match(Token::Tag::LParen)) {
            return false;
        }

        if (!validateCondExpr()) {
            return false;
        }

        return match(Token::Tag::RParen);
    } else if (peek(Token::Tag::LParen)) {
        if (!isCondExprSeparatedByLogicalOperator()) {
            return validateRelExpr();
        }

        // && || exist
        if (!match(Token::Tag::LParen)) {
            return false;
        }

        if (!validateCondExpr()) {
            return false;
        }

        if (!match(Token::Tag::RParen)) {
            return false;
        }

        if (!match(Token::Tag::LogicalAnd) && !match(Token::Tag::LogicalOr)) {
            return false;
        }

        if (!match(Token::Tag::LParen)) {
            return false;
        }

        if (!validateCondExpr()) {
            return false;
        }

        return match(Token::Tag::RParen);
    } else {
        return validateRelExpr();
    }
}

bool SyntaxValidator::validateRelExpr() {
    if (!validateExpr()) {
        return false;
    }

    if (!match(Token::Tag::Equivalence) && !match(Token::Tag::NotEqual) && !match(Token::Tag::LessThan)
        && !match(Token::Tag::LessThanEqualTo) && !match(Token::Tag::GreaterThan)
        && !match(Token::Tag::GreaterThanEqualTo)) {
        return false;
    }

    return validateExpr();
}
