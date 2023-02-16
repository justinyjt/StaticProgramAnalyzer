#include <utility>

#include "SyntaxValidator.h"

SyntaxValidator::SyntaxValidator(std::unique_ptr<ILexer> lex) :
    lex_(std::move(lex)), tokenLst_() {
    cur_ = std::move(lex_->scan());
}

bool SyntaxValidator::validateProgram() {
    while (peek(Token::Tag::Procedure)) {
        tokenLst_.push_front(std::move(cur_));
        next();

        if (!validateProc()) {
            return false;
        }
    }
    if (peek(Token::Tag::EndOfFile)) {
        tokenLst_.push_front(std::move(cur_));
        return true;
    }
    return false;
}

bool SyntaxValidator::validateProc() {
    if (!peek(Token::Tag::Name)) {
        return false;
    }
    tokenLst_.push_front(std::move(cur_));
    next();

    return validateStmtLst();
}

bool SyntaxValidator::validateStmtLst() {
    if (!match(Token::Tag::LBrace)) {
        return false;
    }
    tokenLst_.push_front(std::make_unique<Token>(Token::Tag::LBrace));

    while (!peek(Token::Tag::RBrace)) {
        if (!validateStmt()) {
            return false;
        }
    }

    tokenLst_.push_front(std::make_unique<Token>(Token::Tag::RBrace));
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
        //  std::runtime_error("Unidentified Token");
        return false;
    }
}

bool SyntaxValidator::validateAssign() {
    tokenLst_.push_front(std::move(cur_));
    next();

    tokenLst_.push_front(std::make_unique<Token>(Token::Tag::Assignment));
    if (!match(Token::Tag::Assignment) || !validateExpr()) {
        return false;
    }

    tokenLst_.push_front(std::make_unique<Token>(Token::Tag::SemiColon));
    return match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateRead() {
    tokenLst_.push_front(std::move(cur_));
    next();

    if (!validateName()) {
        return false;
    }

    tokenLst_.push_front(std::make_unique<Token>(Token::Tag::SemiColon));
    return match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validatePrint() {
    tokenLst_.push_front(std::move(cur_));
    next();

    if (!validateName()) {
        return false;
    }

    tokenLst_.push_front(std::make_unique<Token>(Token::Tag::SemiColon));
    return match(Token::Tag::SemiColon);
}

bool SyntaxValidator::validateExpr() {
    if (!validateTerm()) {
        return false;
    }
    if (peek(Token::Tag::Plus)) {
        tokenLst_.push_front(std::move(cur_));
        next();
        return validateExpr();
    } else if (peek(Token::Tag::Minus)) {
        tokenLst_.push_front(std::move(cur_));
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
        tokenLst_.push_front(std::move(cur_));
        next();
        return validateTerm();
    } else if (peek(Token::Tag::Divide)) {
        tokenLst_.push_front(std::move(cur_));
        next();
        return validateTerm();
    } else if (peek(Token::Tag::Modulo)) {
        tokenLst_.push_front(std::move(cur_));
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
        tokenLst_.push_front(std::move(std::make_unique<Token>(Token::Tag::LParen)));
        if (!validateExpr()) {
            return false;
        }
        tokenLst_.push_front(std::move(std::make_unique<Token>(Token::Tag::RParen)));
        return match(Token::Tag::RParen);
    } else {
        std::runtime_error("Factor: syntax error");
        return false;
    }
}

bool SyntaxValidator::validateName() {
    if (!peek(Token::Tag::Name)) {
        return false;
    }
    tokenLst_.push_front(std::move(cur_));
    next();
    return true;
}

bool SyntaxValidator::validateInt() {
    if (!peek(Token::Tag::Integer)) {
        return false;
    }
    tokenLst_.push_front(std::move(cur_));
    next();
    return true;
}

int SyntaxValidator::peek(Token::Tag tag) {
    return (cur_->getTag() == tag) ? 1 : 0;
}

int SyntaxValidator::match(Token::Tag tag) {
    if (cur_->getTag() == Token::Tag::EndOfFile) {
        return tag == Token::Tag::EndOfFile ? 1 : 0;
    }

    if (cur_->getTag() == tag) {
        cur_ = std::move(lex_->scan());
        return 1;
    }
    return 0;
}

void SyntaxValidator::next() {
    cur_ = std::move(lex_->scan());
}

std::deque<std::unique_ptr<Token>> SyntaxValidator::getTokenLst() {
    return std::move(this->tokenLst_);
}
