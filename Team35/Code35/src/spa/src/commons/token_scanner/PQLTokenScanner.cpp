#include "PQLTokenScanner.h"
#include "qps/query_exceptions/SyntaxException.h"

PQLTokenScanner::PQLTokenScanner(std::unique_ptr<ILexer> lex) : TokenScanner(std::move(lex)) {}

PQLTokenScanner::PQLTokenScanner(TokenLst token_lst) : TokenScanner(std::move(token_lst)) {}

int PQLTokenScanner::peekDesignEntity() const {
    return peek(Token::Tag::Statement) || peek(Token::Tag::Read) || peek(Token::Tag::Print) || peek(Token::Tag::Call)
           || peek(Token::Tag::While) || peek(Token::Tag::If) || peek(Token::Tag::Assign) || peek(Token::Tag::Constant)
           || peek(Token::Tag::Procedure) || peek(Token::Tag::Variable);
}

void PQLTokenScanner::matchAndValidate(Token::Tag tag) {
    if (!match(tag)) {
        throw SyntaxException();
    }
}
