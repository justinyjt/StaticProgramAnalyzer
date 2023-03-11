#include "MockExprMaker.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/token_scanner/TokenScanner.h"
#include "commons/expr_parser/ExprParser.h"

ASSIGN_PAT_RIGHT MockExprMaker::makePatternRight(std::string input) {
    std::unique_ptr<ILexer> lxr =
            LexerFactory::createLexer(input, LexerFactory::LexerType::Expression);
    TokenScanner scanner(std::move(lxr));
    ExprParser parser(scanner);
    return parser.parseExpr();
}
