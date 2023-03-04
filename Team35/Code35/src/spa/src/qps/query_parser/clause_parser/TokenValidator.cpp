#include "TokenValidator.h"

#include <vector>

#include "qps/query_exceptions/SyntaxException.h"

TokenValidator::TokenValidator(std::unique_ptr<ILexer> &lexer) : lexer(lexer) {
    cur_ = lexer->scan();
}

std::unique_ptr<Token> TokenValidator::validateAndConsumeDesignEntityToken() {
    if (cur_->getTag() == Token::Tag::Statement ||
        cur_->getTag() == Token::Tag::Read ||
        cur_->getTag() == Token::Tag::Print ||
        cur_->getTag() == Token::Tag::Call ||
        cur_->getTag() == Token::Tag::While ||
        cur_->getTag() == Token::Tag::If ||
        cur_->getTag() == Token::Tag::Assign ||
        cur_->getTag() == Token::Tag::Variable ||
        cur_->getTag() == Token::Tag::Constant ||
        cur_->getTag() == Token::Tag::Procedure) {
        std::unique_ptr<Token> res = std::move(cur_);
        cur_ = lexer->scan();
        return res;
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<Token> TokenValidator::validateAndConsumeSynonymToken() {
    // IDENT
    if (isName(cur_->getLexeme())) {
        std::unique_ptr<Token> res = std::move(cur_);
        cur_ = lexer->scan();
        return res;
    } else {
        throw SyntaxException();
    }
}

std::string TokenValidator::validateAndConsumeRelationship() {
    std::string relationship;
    if (cur_->getTag() == Token::Tag::Modifies ||
        cur_->getTag() == Token::Tag::Uses) {
        relationship += cur_->getLexeme();
        cur_ = lexer->scan();
        return relationship;
    } else if (cur_->getTag() == Token::Tag::Parent ||
        cur_->getTag() == Token::Tag::Follows) {
        relationship += cur_->getLexeme();
        cur_ = lexer->scan();
        if (cur_->getTag() == Token::Tag::Star) {
            relationship += cur_->getLexeme();
            cur_ = lexer->scan();
        }
        return relationship;
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<Token> TokenValidator::validateAndConsumeRelationshipArg() {
    if (cur_->getTag() == Token::Tag::Name ||
        cur_->getTag() == Token::Tag::Underscore ||
        cur_->getTag() == Token::Tag::String ||
        cur_->getTag() == Token::Tag::Integer) {
        std::unique_ptr<Token> res = std::move(cur_);
        cur_ = lexer->scan();
        return res;
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<Token> TokenValidator::validateAndConsumePatternFirstArg() {
    if (cur_->getTag() == Token::Tag::Name ||
        cur_->getTag() == Token::Tag::Underscore ||
        cur_->getTag() == Token::Tag::String) {
        std::unique_ptr<Token> res = std::move(cur_);
        cur_ = lexer->scan();
        return res;
    } else {
        throw SyntaxException();
    }
}

std::vector<std::unique_ptr<Token>> TokenValidator::validateAndConsumePatternSecondArg() {
    std::vector<std::unique_ptr<Token>> tokenList;
    if (cur_->getTag() == Token::Tag::Underscore) {
        tokenList.push_back(std::move(cur_));
        cur_ = lexer->scan();
        if (cur_->getTag() == Token::Tag::String) {
            tokenList.push_back(std::move(cur_));
            cur_ = lexer->scan();
            validateAndConsumeTokenType(Token::Tag::Underscore);
        }
        return tokenList;
    } else {
        throw SyntaxException();
    }
}

bool TokenValidator::isNextTokenType(Token::Tag tag) {
    return tag == cur_->getTag();
}

bool TokenValidator::isNextTokenValidName() {
    return isName(cur_->getLexeme());
}

void TokenValidator::consumeNextToken() {
    cur_ = lexer->scan();
}

bool TokenValidator::isName(std::string input) {
    if (!isalpha(input[0])) {
        return false;
    }
    for (char c : input) {
        if (!isalpha(c) && !isdigit(c)) {
            return false;
        }
    }
    return true;
}

void TokenValidator::validateAndConsumeTokenType(Token::Tag tag) {
    if (isNextTokenType(tag)) {
        consumeNextToken();
    } else {
        throw SyntaxException();
    }
}

bool TokenValidator::isEof() {
    return cur_->getTag() == Token::Tag::EndOfFile;
}
