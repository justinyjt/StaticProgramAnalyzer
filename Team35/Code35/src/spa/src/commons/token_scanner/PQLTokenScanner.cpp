#include "PQLTokenScanner.h"

PQLTokenScanner::PQLTokenScanner(std::unique_ptr<ILexer> lex) : TokenScanner(std::move(lex)) {}

PQLTokenScanner::PQLTokenScanner(TokenLst token_lst) : TokenScanner(std::move(token_lst)) {}

int PQLTokenScanner::peekDesignEntity() const {
    return peek(Token::Tag::Statement) || peek(Token::Tag::Read) || peek(Token::Tag::Print) || peek(Token::Tag::Call)
           || peek(Token::Tag::While) || peek(Token::Tag::If) || peek(Token::Tag::Assign) || peek(Token::Tag::Constant)
           || peek(Token::Tag::Procedure) || peek(Token::Tag::Variable);
}

// for expression with string
int PQLTokenScanner::peekIdent() const {
    std::string input = peekLexeme();
    if (peek(Token::Tag::String)) {
        if (!isalpha(input[0])) {
            return false;
        }
        for (char c : input) {
            if (!isalpha(c) && !isdigit(c)) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

// for expression with constant
int PQLTokenScanner::peekConstant() const {
    std::string next = peekLexeme();
    if (peek(Token::Tag::String)) {
        std::string input = peekLexeme();
        std::string::const_iterator it = input.begin();
        while (it != input.end() && std::isdigit(*it)) ++it;
        return !input.empty() && it == input.end();
    } else {
        return false;
    }
}

bool PQLTokenScanner::peekStmtRef() {
    return peek(Token::Tag::Integer) || peek(Token::Tag::Underscore) ||
           peek(Token::Tag::Bool) || isName();
}

bool PQLTokenScanner::peekEntRef() {
    return peek(Token::Tag::Underscore) || isName() ||
           peek(Token::Tag::Bool) || peekIdent();
}

void PQLTokenScanner::matchAndValidate(Token::Tag tag) {
    if (!match(tag)) {
        throw SyntaxException();
    }
}
