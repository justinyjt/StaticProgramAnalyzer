#include "ExprValidator.h"

ExprValidator::ExprValidator(TokenScanner &scanner) : scanner_(scanner) {}

bool ExprValidator::validateExpr() {
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

bool ExprValidator::validateTerm() {
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

bool ExprValidator::validateFactor() {
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

bool ExprValidator::validateName() {
    if (scanner_.isName()) {
        scanner_.next();
        return true;
    }
    return false;
}

bool ExprValidator::validateInt() {
    return scanner_.match(Token::Tag::Integer);
}
