#include "SimpleTokenScanner.h"

#include <cassert>
#include <memory>
#include <utility>

SimpleTokenScanner::SimpleTokenScanner(std::unique_ptr<ILexer> lex) : TokenScanner(std::move(lex)) {}

SimpleTokenScanner::SimpleTokenScanner(TokenLst token_lst) : TokenScanner(std::move(token_lst)) {}

/**
 * Check if the token is a name.
 * @return 0 if the token is not a name, 1 otherwise.
 */
int SimpleTokenScanner::isName() const {
    return peek(Token::Tag::Name) || peek(Token::Tag::Call) || peek(Token::Tag::Read) || peek(Token::Tag::Print)
        || peek(Token::Tag::Procedure) || peek(Token::Tag::Else) || peek(Token::Tag::Then) || peek(Token::Tag::If)
        || peek(Token::Tag::While);
}

/**
 * Check if the token is the start of a conditional expression separated by a logical operator.
 * @return 0 if the token is not the start of a conditional expression separated by a logical operator, 1 otherwise.
 */
int SimpleTokenScanner::isCondExprSeparatedByLogicalOperator() const {
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

/**
 * Check if the token is the start of an assignment statement.
 * @return 0 if the token is not the start of an assignment statement, 1 otherwise.
 */
int SimpleTokenScanner::isAssign() const {
    return peekOffset(Token::Tag::Assignment, 1) && isName();
}
