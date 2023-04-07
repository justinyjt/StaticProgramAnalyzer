/*
 * 1. Add more negative cases
 * 2. Add testing to tokenLst
 * */
#include "catch.hpp"
#include "commons/token/Token.h"
#include "../commons/lexer/MockLexer.h"
#include "../TestHelper.h"
#include "sp/SourceValidator.h"

TEST_CASE("SourceValidator can validate very simple program correctly") {
    SECTION("SV can validate blank program") {
        Token eof(Token::Tag::EndOfFile);
        std::vector<Token> tokens = {eof};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate one empty procedure") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);

        std::vector<Token> tokens = {eof, rBrace, lBrace, procName, proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireFalse(sv.validateProgram());
    }

    SECTION("SV can validate one procedure with one print") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token printStmt(Token::Tag::Print);
        Token varName("x", Token::Tag::Name);

        std::vector<Token> tokens = {eof, rBrace, semi, varName, printStmt, lBrace, procName, proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate one procedure with one read") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName("x", Token::Tag::Name);

        std::vector<Token> tokens = {eof, rBrace, semi, varName, readStmt, lBrace, procName, proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate one procedure with one read") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token callStmt(Token::Tag::Call);
        Token varName("x", Token::Tag::Name);

        std::vector<Token> tokens = {eof, rBrace, semi, varName, callStmt, lBrace, procName, proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }
}

TEST_CASE("SV can validate assign statement correctly") {
    SECTION("SV can validate x = 1;") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token varName("x", Token::Tag::Name);
        Token assignment("=", Token::Tag::Assignment);
        Token constVal("1", Token::Tag::Integer);

        std::vector<Token> tokens = {eof, rBrace, semi, constVal, assignment, varName, lBrace, procName, proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate x = y;") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token varName1("x", Token::Tag::Name);
        Token assignment("=", Token::Tag::Assignment);
        Token varName2("y", Token::Tag::Name);

        std::vector<Token> tokens = {eof, rBrace, semi, varName2, assignment, varName1, lBrace, procName, proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate x = x + 1;") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token varName1("x", Token::Tag::Name);
        Token assignment("=", Token::Tag::Assignment);
        Token plus("+", Token::Tag::Plus);
        Token constVal("1", Token::Tag::Integer);

        std::vector<Token> tokens = {
            eof, rBrace,
            semi, constVal, plus, varName1, assignment, varName1,
            lBrace, procName, proc
        };

        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate x = x * 1;") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token varName1("x", Token::Tag::Name);
        Token assignment("=", Token::Tag::Assignment);
        Token mul("*", Token::Tag::Multiply);
        Token constVal("1", Token::Tag::Integer);

        std::vector<Token> tokens = {
            eof, rBrace,
            semi, constVal, mul, varName1, assignment, varName1,
            lBrace, procName, proc
        };

        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate x = ((x + 1) % (y - z) + (2 * 3)) / x;") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token lParen(Token::Tag::LParen);
        Token rParen(Token::Tag::RParen);
        Token semi(Token::Tag::SemiColon);
        Token assignment("=", Token::Tag::Assignment);
        Token plus("+", Token::Tag::Plus);
        Token minus("-", Token::Tag::Minus);
        Token mul("+", Token::Tag::Multiply);
        Token div("/", Token::Tag::Divide);
        Token mod("%", Token::Tag::Modulo);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token varName3("z", Token::Tag::Name);
        Token constVal1("1", Token::Tag::Integer);
        Token constVal2("2", Token::Tag::Integer);
        Token constVal3("3", Token::Tag::Integer);

        std::vector<Token> tokens = {
            eof, rBrace,
            semi, varName1, div, rParen, rParen, constVal3, mul, constVal2, lParen, plus,
            rParen, varName3, minus, varName2, lParen, mod, rParen, constVal1, plus, varName1, lParen, lParen,
            assignment, varName1,
            lBrace, procName, proc
        };

        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }
}

TEST_CASE("SV can validate multiple stmt program") {
    SECTION("SV can validate two read stmt") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);

        std::vector<Token> tokens = {
            eof, rBrace,
            semi, varName2, readStmt,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));

        requireTrue(sv.validateProgram());
    }
}

TEST_CASE("SV can validate if clauses") {
    SECTION("SV can validate single if clause") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid if clause (missing right brace)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace,
            semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid if clause (missing left brace)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid if clause (missing right parenthesis)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }SECTION("SV can invalidate invalid if clause (missing left parenthesis)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, constVal1, equiv, rightParent, varName1, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid if clause (missing then)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid if clause (missing else clause)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can validate two if clauses") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate two if clauses") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate three if clauses") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token ifDeclaration(Token::Tag::If);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token constVal1("1", Token::Tag::Integer);
        Token then(Token::Tag::Then);
        Token els(Token::Tag::Else);
        std::vector<Token> tokens = {
            eof, rBrace,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            rBrace, semi, varName1, readStmt,
            lBrace, els, rBrace,
            semi, varName2, readStmt,
            lBrace, then, rightParent, constVal1, equiv, varName1, leftParent, ifDeclaration,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }
}

TEST_CASE("SV can validate while clauses") {
    SECTION("SV can validate single while clauses") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, leftParent, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid while clause (missing left parenthesis)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid while clause (missing right parenthesis)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, varName2, equiv, varName1, leftParent, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid while clause (invalid cond_expr)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, rightParent, varName2, varName1, leftParent, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid while clause (invalid cond_expr 2)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, rightParent, equiv, varName1, leftParent, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate invalid while clause (invalid cond_expr 3)") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace, rBrace,
            semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, leftParent, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can validate two while clauses") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace,
            rBrace, semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, leftParent, whileToken,
            rBrace, semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, leftParent, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate two nested while clauses") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace,
            rBrace, rBrace, semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, leftParent, whileToken,
            semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, leftParent, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate three nested while clauses") {
        Token eof(Token::Tag::EndOfFile);
        Token proc(Token::Tag::Procedure);
        Token procName("main", Token::Tag::Name);
        Token lBrace(Token::Tag::LBrace);
        Token rBrace(Token::Tag::RBrace);
        Token semi(Token::Tag::SemiColon);
        Token readStmt(Token::Tag::Read);
        Token varName1("x", Token::Tag::Name);
        Token varName2("y", Token::Tag::Name);
        Token equiv(Token::Tag::Equivalence);
        Token leftParent(Token::Tag::LParen);
        Token rightParent(Token::Tag::RParen);
        Token whileToken(Token::Tag::While);
        std::vector<Token> tokens = {
            eof, rBrace,
            rBrace, rBrace,
            rBrace, semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, leftParent, whileToken,
            semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, leftParent, whileToken,
            semi, varName1, readStmt,
            lBrace, rightParent, varName2, equiv, varName1, leftParent, whileToken,
            lBrace, procName, proc
        };
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }
}

TEST_CASE("SV can handle conditional expressions well", "[sv]") {
    SECTION("SV can validate if clause: if(((x>0)&&(x>0))||(!(x>0)))then{x=2;}else{x=2;}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        Token Minus(Token::Tag::Minus);
        Token Not(Token::Tag::LogicalNot);
        Token Assign(Token::Tag::Assignment);
        Token Semi(Token::Tag::SemiColon);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Else,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Then,
            RParen, RParen,
            RParen, ConstVal, GreaterThan, VarName, LParen, Not, LParen, LogicalOr,
            RParen, RParen, ConstVal, GreaterThan, VarName, LParen, LogicalAnd, RParen, ConstVal, GreaterThan, VarName,
            LParen, LParen,
            LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }

    SECTION("SV can validate if clause: if(((x-2)>2)&&((x)>0))then{x=2;}else{x=2;}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        Token Minus(Token::Tag::Minus);
        Token Assign(Token::Tag::Assignment);
        Token Semi(Token::Tag::SemiColon);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Else,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Then,
            RParen, RParen, ConstVal, GreaterThan, RParen, VarName, LParen, LParen, LogicalAnd, RParen, ConstVal,
            GreaterThan, RParen, ConstVal,
            Minus, VarName, LParen,
            LParen, LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireTrue(sv.validateProgram());
    }

    SECTION("SV can invalidate if clause: if((x-2>2)&&(x))then{x=2;}else{x=2;}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        Token Minus(Token::Tag::Minus);
        Token Assign(Token::Tag::Assignment);
        Token Semi(Token::Tag::SemiColon);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Else,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Then,
            RParen, RParen, VarName, LParen, LogicalAnd, RParen, ConstVal, GreaterThan, ConstVal,
            Minus, VarName,
            LParen, LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate if clause: if((x-2>2)&&x>2))then{x=2;}else{x=2;}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        Token Minus(Token::Tag::Minus);
        Token Assign(Token::Tag::Assignment);
        Token Semi(Token::Tag::SemiColon);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Else,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Then,
            RParen, RParen, ConstVal, GreaterThan, VarName, LogicalAnd, RParen, ConstVal, GreaterThan, ConstVal,
            Minus, VarName,
            LParen, LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate if clause: if((x-2>2&&(x>0))then{x=2;}else{x=2;}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        Token Minus(Token::Tag::Minus);
        Token Assign(Token::Tag::Assignment);
        Token Semi(Token::Tag::SemiColon);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Else,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Then,
            RParen, RParen, ConstVal, GreaterThan, VarName, LParen, LogicalAnd, ConstVal, GreaterThan, ConstVal,
            Minus, VarName,
            LParen, LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate if clause: if((x-2>2&&(x>0))then{x=2;}else{x=2;}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        Token Minus(Token::Tag::Minus);
        Token Assign(Token::Tag::Assignment);
        Token Semi(Token::Tag::SemiColon);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Else,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Then,
            RParen, RParen,
            ConstVal, GreaterThan, VarName, LParen, LogicalAnd, ConstVal, GreaterThan, ConstVal,
            Minus, VarName,
            LParen, LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate if clause: if(x-2>2)&&(x>0))then{x=2;}else{x=2;}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        Token Minus(Token::Tag::Minus);
        Token Not(Token::Tag::LogicalNot);
        Token Assign(Token::Tag::Assignment);
        Token Semi(Token::Tag::SemiColon);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Else,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Then,
            RParen, RParen,
            ConstVal, GreaterThan, VarName, LParen, LogicalAnd, RParen, ConstVal, GreaterThan, ConstVal,
            Minus, VarName,
            LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate if clause: if((x-2>)&&(x>0))then{x=2;}else{x=2;}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        Token Minus(Token::Tag::Minus);
        Token Not(Token::Tag::LogicalNot);
        Token Assign(Token::Tag::Assignment);
        Token Semi(Token::Tag::SemiColon);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Else,
            RBrace, Semi, ConstVal, Assign, VarName, LBrace, Then,
            RParen, RParen,
            ConstVal, GreaterThan, VarName, LParen, LogicalAnd, RParen, GreaterThan, ConstVal,
            Minus, VarName,
            LParen, LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }
}

TEST_CASE("SV can invalidate empty stmt list") {
    SECTION("SV can invalidate empty procedure: procedure main(){}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        std::vector<Token> tokens = {
            EoF, RBrace,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate empty if clause: if(x>2)then{}else{}") {
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
        Token ConstVal("2", Token::Tag::Integer);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, RBrace, LBrace, Else,
            RBrace, LBrace, Then,
            RParen, RParen,
            ConstVal, GreaterThan, VarName, LParen, If,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }

    SECTION("SV can invalidate empty while clause: while(x>2){}") {
        Token EoF(Token::Tag::EndOfFile);
        Token Proc(Token::Tag::Procedure);
        Token ProcName("main", Token::Tag::Name);
        Token LBrace(Token::Tag::LBrace);
        Token RBrace(Token::Tag::RBrace);
        Token LParen(Token::Tag::LParen);
        Token RParen(Token::Tag::RParen);
        Token While(Token::Tag::While);
        Token VarName("x", Token::Tag::Name);
        Token GreaterThan(">", Token::Tag::GreaterThan);
        Token ConstVal("2", Token::Tag::Integer);
        std::vector<Token> tokens = {
            EoF, RBrace,
            RBrace, LBrace,
            RParen, RParen,
            ConstVal, GreaterThan, VarName, LParen, While,
            LBrace, ProcName, Proc};
        MockLexer lex(tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SourceValidator sv(std::move(lexPtr));
        requireFalse(sv.validateProgram());
    }
}
