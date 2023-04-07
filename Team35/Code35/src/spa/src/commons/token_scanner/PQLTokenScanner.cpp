#include "PQLTokenScanner.h"

PQLTokenScanner::PQLTokenScanner(std::unique_ptr<ILexer> lex) : TokenScanner(std::move(lex)) {}

int PQLTokenScanner::peekDesignEntity() const {
    return peek(Token::Tag::Statement) || peek(Token::Tag::Read) || peek(Token::Tag::Print) || peek(Token::Tag::Call)
            || peek(Token::Tag::While) || peek(Token::Tag::If) || peek(Token::Tag::Assign) || peek(Token::Tag::Constant)
            || peek(Token::Tag::Procedure) || peek(Token::Tag::Variable);
}

int PQLTokenScanner::peekSuchThatClause() const {
    return peek(Token::Tag::Such) && peekOffset(Token::Tag::That, 1);
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
