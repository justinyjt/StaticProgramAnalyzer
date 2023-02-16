#include <utility>

#include "SyntaxValidator.h"

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
    if (!match(Token::Tag::Name)) {
        return false;
    }

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
    if (peek(Token::Tag::Name)) {
        return validateAssign();
    } else if (peek(Token::Tag::Read)) {
        return validateRead();
    } else if (peek(Token::Tag::Print)) {
        return validatePrint();
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
    if (peek(Token::Tag::Name)) {
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
    if (peek(Token::Tag::Name) || peek(Token::Tag::Call) || peek(Token::Tag::Read) || peek(Token::Tag::Print)
        || peek(Token::Tag::Procedure) || peek(Token::Tag::Else) || peek(Token::Tag::Then) || peek(Token::Tag::If)
        || peek(Token::Tag::While)) {
        next();
        return true;
    }
    return false;
}

bool SyntaxValidator::validateInt() {
    return match(Token::Tag::Integer);
}

int SyntaxValidator::peek(Token::Tag tag) {
    return (token_lst_[cur_idx_]->getTag() == tag) ? 1 : 0;
}

int SyntaxValidator::match(Token::Tag tag) {
    if (token_lst_[cur_idx_]->getTag() == tag) {
        if (tag != Token::Tag::EndOfFile) {
            next();
        }
        return 1;
    }
    return 0;
}

void SyntaxValidator::next() {
    ++cur_idx_;
}

std::deque<std::unique_ptr<Token>> SyntaxValidator::getTokenLst() {
    std::reverse(token_lst_.begin(), token_lst_.end());
    return std::move(this->token_lst_);
}
