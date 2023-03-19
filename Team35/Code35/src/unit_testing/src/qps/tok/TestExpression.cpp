#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Expression.h"
#include "qps/pql/PQLToken.h"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/expr_parser/ExprParser.h"

TEST_CASE("Expression") {
    std::string exprStr1 = "x + 1";
    std::unique_ptr<ILexer> lxr1 =
            LexerFactory::createLexer(exprStr1, LexerFactory::LexerType::Expression);
    TokenScanner scanner1(std::move(lxr1));
    ExprParser parser1(scanner1);
    ASSIGN_PAT_RIGHT pattern1 = parser1.parseExpr();

    std::string exprStr2 = "y + 1";
    std::unique_ptr<ILexer> lxr2 =
            LexerFactory::createLexer(exprStr2, LexerFactory::LexerType::Expression);
    TokenScanner scanner2(std::move(lxr2));
    ExprParser parser2(scanner2);
    ASSIGN_PAT_RIGHT pattern2 = parser2.parseExpr();

    Expression *st1 = new Expression(pattern1, false);
    Expression *st2 = new Expression(pattern2, false);
    std::unique_ptr<PQLToken> st1_ = std::make_unique<Expression>(pattern1, false);

    requireTrue(*st1 == *st1_);
    requireTrue(*st1 != *st2);

    requireEqual(st1->tag, PQLToken::Tag::EXPR);
    delete st1;
    delete st2;
}
