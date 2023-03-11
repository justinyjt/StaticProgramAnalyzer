#include "catch.hpp"
#include "sp/Parser.h"
#include "ASTPrinter.h"
#include "sp/SourceProcessor.h"

std::unique_ptr<Token> transform(const Token &token) {
    return std::make_unique<Token>(token);
}

void convertDequeReverse(const std::deque<Token> &va, std::deque<std::unique_ptr<Token>> &vb) {
    vb.clear();
    std::transform(va.begin(), va.end(), std::front_inserter(vb), transform);
}

void convertDeque(const std::deque<Token> &va, std::deque<std::unique_ptr<Token>> &vb) {
    vb.clear();
    std::transform(va.begin(), va.end(), std::back_inserter(vb), transform);
}

TEST_CASE("Parser can parse assignment correctly", "[Parser]") {
    SECTION("Parser can parse blank program") {
        Token eof(Token::Tag::EndOfFile);
        std::deque<Token> tokens = {eof};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;

        REQUIRE(printer.printAST(root).empty());
    }

    SECTION("Parser can parse one empty procedure") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        std::deque<Token> tokens = {EoF, RBrace, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;

        REQUIRE(printer.printAST(root) == "procedure main {\n}\n");
    }

    SECTION("Parser can parse one procedure with x = 1;") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token VarName("x", Token::Tag::Name);
        Token Assignment("=", Token::Tag::Assignment);
        Token ConstVal("1", Token::Tag::Integer);
        Token Semi(Token::Tag::SemiColon);
        Token RBrace(Token::Tag::RBrace);
        std::deque<Token> tokens = {EoF, RBrace, Semi, ConstVal, Assignment, VarName, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;

        REQUIRE(printer.printAST(root) == "procedure main {\nx=1;\n}\n");
    }

    SECTION("Parser can parse one procedure with x = 1 + y * 22;") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token VarName1("x", Token::Tag::Name);
        Token Assignment("=", Token::Tag::Assignment);
        Token ConstVal1("1", Token::Tag::Integer);
        Token Plus("+", Token::Tag::Plus);
        Token VarName2("y", Token::Tag::Name);
        Token Mul("*", Token::Tag::Multiply);
        Token ConstVal2("22", Token::Tag::Integer);
        Token Semi(Token::Tag::SemiColon);
        Token RBrace(Token::Tag::RBrace);
        std::deque<Token> tokens = {EoF, RBrace, Semi, ConstVal2, Mul, VarName2, Plus, ConstVal1,
                                    Assignment, VarName1, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;

        REQUIRE(printer.printAST(root) == "procedure main {\nx=1+y*22;\n}\n");
    }

    SECTION("Parser can parse one procedure with x = (y % 123) / 456;") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token VarName1("x", Token::Tag::Name);
        Token Assignment("=", Token::Tag::Assignment);
        Token ConstVal1("123", Token::Tag::Integer);
        Token Mod("%", Token::Tag::Modulo);
        Token VarName2("y", Token::Tag::Name);
        Token Div("/", Token::Tag::Divide);
        Token ConstVal2("456", Token::Tag::Integer);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token Semi(Token::Tag::SemiColon);
        Token RBrace(Token::Tag::RBrace);
        std::deque<Token> tokens = {EoF, RBrace, Semi, ConstVal2, Div, RParen, ConstVal1, Mod, VarName2,
                                    LParen, Assignment, VarName1, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;

        REQUIRE(printer.printAST(root) == "procedure main {\nx=y%123/456;\n}\n");
    }

    SECTION("Parser can parse one procedure with K0koM0 = ((14 * (1926 % 817)) / (zZz + r4t));") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token Semi(Token::Tag::SemiColon);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token Assignment("=", Token::Tag::Assignment);
        Token VarName1("K0koM0", Token::Tag::Name);
        Token VarName2("zZz", Token::Tag::Name);
        Token VarName3("r4t", Token::Tag::Name);
        Token ConstVal1("14", Token::Tag::Integer);
        Token ConstVal2("1926", Token::Tag::Integer);
        Token ConstVal3("817", Token::Tag::Integer);
        Token Plus("+", Token::Tag::Plus);
        Token Mul("*", Token::Tag::Multiply);
        Token Mod("%", Token::Tag::Modulo);
        Token Div("/", Token::Tag::Divide);
        std::deque<Token> tokens = {EoF, RBrace, Semi, RParen, RParen, VarName3, Plus, VarName2, LParen,
                                    Div, RParen, RParen, ConstVal3, Mod, ConstVal2, LParen, Mul, ConstVal1, LParen,
                                    LParen, Assignment, VarName1, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;

        REQUIRE(printer.printAST(root) == "procedure main {\nK0koM0=14*1926%817/zZz+r4t;\n}\n");
    }

    SECTION("Parser can parse one procedure with x = (y % 123) / 456; y = 456;") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token VarName1("x", Token::Tag::Name);
        Token Assignment("=", Token::Tag::Assignment);
        Token ConstVal1("123", Token::Tag::Integer);
        Token Mod("%", Token::Tag::Modulo);
        Token VarName2("y", Token::Tag::Name);
        Token Div("/", Token::Tag::Divide);
        Token ConstVal2("456", Token::Tag::Integer);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token Semi(Token::Tag::SemiColon);
        Token RBrace(Token::Tag::RBrace);
        std::deque<Token> tokens = {EoF, RBrace, Semi, ConstVal2, Assignment, VarName2,
                                    Semi, ConstVal2, Div, RParen, ConstVal1, Mod, VarName2,
                                    LParen, Assignment, VarName1, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nx=y%123/456;\ny=456;\n}\n");
    }
}

TEST_CASE("Parser can parse read correctly", "[Parser]") {
    SECTION("Parser can parse one read stmt: read x;") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token Semi(Token::Tag::SemiColon);
        Token ReadStmt(Token::Tag::Read);
        Token VarName("x", Token::Tag::Name);
        std::deque<Token> tokens = {EoF, RBrace, Semi, VarName, ReadStmt, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);
        tokens.clear();

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nread x;\n}\n");
    }
}

TEST_CASE("Parser can parse print correctly", "[Parser]") {
    SECTION("Parser can parse one print stmt: print x;") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token Semi(Token::Tag::SemiColon);
        Token PrintStmt(Token::Tag::Print);
        Token VarName("x", Token::Tag::Name);
        std::deque<Token> tokens = {EoF, RBrace, Semi, VarName, PrintStmt, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;

        REQUIRE(printer.printAST(root) == "procedure main {\nprint x;\n}\n");
    }
}

TEST_CASE("Parser can parse conditional expression correctly", "[Parser]") {
    SECTION("Parser can parse: if(x>0){}then{}else{}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);
        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, LBrace, Else,
                RBrace, LBrace, Then,
                RParen, ConstVal, GreaterThan, VarName, LParen, If,
                LBrace, ProcName, Proc
        };

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nif (x>0) then {\n} else {\n}\n}\n");
    }

    SECTION("Parser can parse: if(x>(0+0)){}then{}else{}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token VarName("x", Token::Tag::Name);
        Token Plus("+", Token::Tag::Plus);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);
        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, LBrace, Else,
                RBrace, LBrace, Then,
                RParen, RParen, ConstVal, Plus, ConstVal, LParen, GreaterThan, VarName, LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nif (x>0+0) then {\n} else {\n}\n}\n");
    }

    SECTION("Parser can parse: if(!(x>0)){}then{}else{}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token VarName("x", Token::Tag::Name);
        Token LogicalNot("+", Token::Tag::LogicalNot);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);
        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, LBrace, Else,
                RBrace, LBrace, Then,
                RParen, RParen, ConstVal, GreaterThan, VarName, LParen, LogicalNot, LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nif (!(x>0)) then {\n} else {\n}\n}\n");
    }

    SECTION("Parser can parse: if((x>0)&&(x>0))then{}else{}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token VarName("x", Token::Tag::Name);
        Token LogicalAnd("&&", Token::Tag::LogicalAnd);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);
        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, LBrace, Else,
                RBrace, LBrace, Then,
                RParen, RParen, ConstVal, GreaterThan, VarName, LParen, LogicalAnd, RParen, ConstVal, GreaterThan,
                VarName, LParen, LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nif ((x>0)&&(x>0)) then {\n} else {\n}\n}\n");
    }

    SECTION("Parser can parse: if(((x>0)&&(x>0))||(x>0))then{}else{}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token VarName("x", Token::Tag::Name);
        Token LogicalAnd("&&", Token::Tag::LogicalAnd);
        Token LogicalOr("||", Token::Tag::LogicalOr);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);
        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, LBrace, Else,
                RBrace, LBrace, Then,
                RParen,
                RParen, ConstVal, GreaterThan, VarName, LParen, LogicalOr,
                RParen, RParen, ConstVal, GreaterThan, VarName, LParen, LogicalAnd, RParen, ConstVal, GreaterThan,
                VarName, LParen, LParen,
                LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nif (((x>0)&&(x>0))||(x>0)) then {\n} else {\n}\n}\n");
    }
}

TEST_CASE("Parser can parse if..then...else... correctly", "[Parser]") {
    SECTION("Parser can parse if(x>0)then{read x;}else{}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token Semi(Token::Tag::SemiColon);
        Token ReadStmt(Token::Tag::Read);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);

        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, LBrace, Else,
                RBrace, Semi, VarName, ReadStmt, LBrace, Then,
                RParen, ConstVal, GreaterThan, VarName, LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nif (x>0) then {\nread x;\n} else {\n}\n}\n");
    }

    SECTION("Parser can parse if(x>0)then{}else{read x;}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token Semi(Token::Tag::SemiColon);
        Token ReadStmt(Token::Tag::Read);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);

        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, Semi, VarName, ReadStmt, LBrace, Else,
                RBrace, LBrace, Then,
                RParen, ConstVal, GreaterThan, VarName, LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nif (x>0) then {\n} else {\nread x;\n}\n}\n");
    }

    SECTION("Parser can parse if(x>0)then{read x;}else{read x;}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token Semi(Token::Tag::SemiColon);
        Token ReadStmt(Token::Tag::Read);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);

        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, Semi, VarName, ReadStmt, LBrace, Else,
                RBrace, Semi, VarName, ReadStmt, LBrace, Then,
                RParen, ConstVal, GreaterThan, VarName, LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nif (x>0) then {\nread x;\n} else {\nread x;\n}\n}\n");
    }

    SECTION("Parser can parse if(x>0)then{read x;read x;read x;}else{read x;}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token Semi(Token::Tag::SemiColon);
        Token ReadStmt(Token::Tag::Read);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);

        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, Semi, VarName, ReadStmt, LBrace, Else,
                RBrace, Semi, VarName, ReadStmt, Semi, VarName, ReadStmt, Semi, VarName, ReadStmt, LBrace, Then,
                RParen, ConstVal, GreaterThan, VarName, LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) ==
                "procedure main {\nif (x>0) then {\nread x;\nread x;\nread x;\n} else {\nread x;\n}\n}\n");
    }

    SECTION("Parser can parse nested if: if(x>0)then{if(x>0)then{read x;}else{read x;}}else{read x;}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token If(Token::Tag::If);
        Token Then(Token::Tag::Then);
        Token Else(Token::Tag::Else);
        Token Semi(Token::Tag::SemiColon);
        Token ReadStmt(Token::Tag::Read);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);

        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, Semi, VarName, ReadStmt, LBrace, Else,
                RBrace,
                RBrace, Semi, VarName, ReadStmt, LBrace, Else,
                RBrace, Semi, VarName, ReadStmt, LBrace, Then,
                RParen, ConstVal, GreaterThan, VarName, LParen, If,
                Then,
                RParen, ConstVal, GreaterThan, VarName, LParen, If,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) ==
                "procedure main {\nif (x>0) then {\nif (x>0) then {\nread x;\n} else {\nread x;\n}\n} else {\nread x;\n}\n}\n");
    }
}

TEST_CASE("Parser can parse while... correctly", "[Parser]") {
    SECTION("Parser can parse while(x>0){read x;}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token While(Token::Tag::While);
        Token Semi(Token::Tag::SemiColon);
        Token ReadStmt(Token::Tag::Read);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);

        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, Semi, VarName, ReadStmt, LBrace,
                RParen, ConstVal, GreaterThan, VarName, LParen, While,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nwhile (x>0) {\nread x;\n}\n}\n");
    }

    SECTION("Parser can parse while(x>0){read x;read x;read x;}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token While(Token::Tag::While);
        Token Semi(Token::Tag::SemiColon);
        Token ReadStmt(Token::Tag::Read);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("0", Token::Tag::Integer);

        std::deque<Token> tokens = {
                EoF, RBrace,
                RBrace, Semi, VarName, ReadStmt, Semi, VarName, ReadStmt, Semi, VarName, ReadStmt, LBrace,
                RParen, ConstVal, GreaterThan, VarName, LParen, While,
                LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;
        REQUIRE(printer.printAST(root) == "procedure main {\nwhile (x>0) {\nread x;\nread x;\nread x;\n}\n}\n");
    }
}

TEST_CASE("Parser can parse call correctly", "[Parser]") {
    SECTION("Parser can parse one call stmt: call main;") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token Semi(Token::Tag::SemiColon);
        Token CallStmt(Token::Tag::Call);
        Token VarName("main", Token::Tag::Name);
        std::deque<Token> tokens = {EoF, RBrace, Semi, VarName, CallStmt, LBrace, ProcName, Proc};

        std::deque<std::unique_ptr<Token>> tokenLst;
        convertDequeReverse(tokens, tokenLst);

        std::unique_ptr<IParser> parser = std::make_unique<Parser>(std::move(tokenLst));
        std::shared_ptr<ASTNode> root = parser->Parse();

        ASTPrinter printer;

        REQUIRE(printer.printAST(root) == "procedure main {\ncall main;\n}\n");
    }
}
