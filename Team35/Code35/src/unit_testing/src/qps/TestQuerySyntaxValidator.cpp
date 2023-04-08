#include "catch.hpp"
#include "../TestHelper.h"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/query_parser/QuerySyntaxValidator.h"
#include "commons/lexer/exception/LexerException.h"
#include "commons/token_scanner/PQLTokenScanner.h"

TEST_CASE("Test declarations") {
    SECTION("invalid design entity") {
        std::string query = "assn s; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("valid synonym") {
        std::string query = "stmt s; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid BOOLEAN") {
        std::string query = "stmt BOOLEAN; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("invalid synonym") {
        std::string query = "stmt +s; Select s";
        requireThrowAs<LexerException>([&query]() {
            std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
            PQLTokenScanner pqlTokenScanner(std::move(lexer));
            std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
            sv->validateQuery();
        });
    }

    SECTION("invalid synonym") {
        std::string query = "stmt 1; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("no semicolon") {
        std::string query = "stmt a assign Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("multiple synonym, 1 design entity") {
        std::string query = "stmt a, b; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("multiple synonym, 1 design entity, contains BOOLEAN") {
        std::string query = "stmt s, BOOLEAN; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("multiple synonym, 1 design entity, invalid second synonym") {
        std::string query = "stmt s, 1; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("multiple design entities") {
        std::string query = "stmt s; assign a; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }
}

TEST_CASE("Test select") {
    SECTION("valid select synonym") {
        std::string query = "stmt s; assign a; Select s";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("invalid select synonym") {
        std::string query = "stmt s; assign a; Select >";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("valid select BOOLEAN") {
        std::string query = "stmt s; assign a; Select BOOLEAN";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid select tuple") {
        std::string query = "stmt s; assign a; Select <BOOLEAN, a>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("invalid select tuple, expected <") {
        std::string query = "stmt s; assign a; Select >BOOLEAN, a>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid select tuple, expected synonym first arg") {
        std::string query = "stmt s; assign a; Select <_, a>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid select tuple, expected ,") {
        std::string query = "stmt s; assign a; Select <a< a>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid select tuple, expected synonym second arg") {
        std::string query = "stmt s; assign a; Select <BOOLEAN, _>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid select tuple, expected >") {
        std::string query = "stmt s; assign a; Select <BOOLEAN, a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }
}

TEST_CASE("Test clause") {
    SECTION("invalid clause, expected with, such that or pattern") {
        std::string query = "stmt s; assign a; Select s stmt";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid such that clause") {
        std::string query = "stmt s; assign a; Select s such that stmt";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("valid modifies") {
        std::string query = "stmt s; assign a; Select s such that Modifies(1,s)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid uses") {
        std::string query = "stmt s; assign a; Select s such that Uses(1,s)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid parent") {
        std::string query = "stmt s; assign a; Select s such that Parent(1,2)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid parent*") {
        std::string query = "stmt s; assign a; Select s such that Parent*(1,2)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid follows") {
        std::string query = "stmt s; assign a; Select s such that Follows(1,2)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid follows*") {
        std::string query = "stmt s; assign a; Select s such that Follows*(1,2)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid calls") {
        std::string query = "stmt s; assign a; Select s such that Calls(s,a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid calls*") {
        std::string query = "stmt s; assign a; Select s such that Calls*(s,a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }
}

TEST_CASE("Test stmt stmt ref") {
    SECTION("invalid expected (") {
        std::string query = "stmt s; assign a; Select s such that Parent*s,a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid first stmt ref") {
        std::string query = "stmt s; assign a; Select s such that Parent*(<,a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected ,") {
        std::string query = "stmt s; assign a; Select s such that Parent*(s(a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid second stmt ref") {
        std::string query = "stmt s; assign a; Select s such that Parent*(s,>)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected )") {
        std::string query = "stmt s; assign a; Select s such that Parent*(s,a>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }
}

TEST_CASE("Test ent ent ref") {
    SECTION("invalid expected (") {
        std::string query = "stmt s; assign a; Select s such that Calls*s,a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid first stmt ref") {
        std::string query = "stmt s; assign a; Select s such that Calls*(<,a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected ,") {
        std::string query = "stmt s; assign a; Select s such that Calls*(s(a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid second stmt ref") {
        std::string query = "stmt s; assign a; Select s such that Calls*(s,>)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected )") {
        std::string query = "stmt s; assign a; Select s such that Calls*(s,a>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }
}

TEST_CASE("Test UsesModifies clauses") {
    SECTION("invalid expected (") {
        std::string query = "stmt s; assign a; Select s such that Modifies<s,a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid first stmt ref") {
        std::string query = "stmt s; assign a; Select s such that Uses(<,a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected ,") {
        std::string query = "stmt s; assign a; Select s such that Modifies(s(a)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid second stmt ref") {
        std::string query = "stmt s; assign a; Select s such that Uses(\"x\",>)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected )") {
        std::string query = "stmt s; assign a; Select s such that Modifies(s,a>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }
}

TEST_CASE("Test assign and while pattern") {
    SECTION("valid entref is synonym") {
        std::string query = "stmt s; assign a; Select s pattern w(s,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid entref is wildcard") {
        std::string query = "stmt s; assign a; Select s pattern w(_,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid entref is ident") {
        std::string query = "stmt s; assign a; Select s pattern w(\"x\",_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid second arg is expr") {
        std::string query = "stmt s; assign a; Select s pattern w(\"x\",\"x + 1\")";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid second arg is _expr_") {
        std::string query = "stmt s; assign a; Select s pattern w(\"x\",_\"x + 1\"_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("invalid _expr_") {
        std::string query = "stmt s; assign a; Select s pattern w(\"x\",_\"+ 1\"_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected synonym") {
        std::string query = "stmt s; assign a; Select s pattern <(s,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected (") {
        std::string query = "stmt s; assign a; Select s pattern w*s,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid first stmt ref") {
        std::string query = "stmt s; assign a; Select s pattern w(<,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected ,") {
        std::string query = "stmt s; assign a; Select s pattern w(s(_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid second stmt ref") {
        std::string query = "stmt s; assign a; Select s pattern w(s,>)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected )") {
        std::string query = "stmt s; assign a; Select s pattern w(s,_>";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }
}

TEST_CASE("Test if pattern") {
    SECTION("valid entref is synonym") {
        std::string query = "stmt s; assign a; Select s pattern ifs(s,_,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid entref is wildcard") {
        std::string query = "stmt s; assign a; Select s pattern ifs(_,_,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid entref is ident") {
        std::string query = "stmt s; assign a; Select s pattern ifs(\"x\",_,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("invalid expected synonym") {
        std::string query = "stmt s; assign a; Select s pattern <(s,_,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected (") {
        std::string query = "stmt s; assign a; Select s pattern ifs*s,_,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid first stmt ref") {
        std::string query = "stmt s; assign a; Select s pattern ifs(<,_,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected ,") {
        std::string query = "stmt s; assign a; Select s pattern ifs(s(_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid second arg") {
        std::string query = "stmt s; assign a; Select s pattern ifs(s,>,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected second ,") {
        std::string query = "stmt s; assign a; Select s pattern ifs(s,_>_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid third arg") {
        std::string query = "stmt s; assign a; Select s pattern ifs(s,_,>)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected )") {
        std::string query = "stmt s; assign a; Select s pattern ifs(s,_,_*";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }
}

TEST_CASE("Test with") {
    SECTION("valid ident integer") {
        std::string query = "stmt s; assign a; Select s with \"s\" = 1";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid attrRef.procName attrRef.value") {
        std::string query = "stmt s; assign a; Select s with s.procName = s.value";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid attrRef.value attrRef.stmt#") {
        std::string query = "stmt s; assign a; Select s with s.value = s.stmt#";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("invalid first attrRef, not a synonym") {
        std::string query = "stmt s; assign a; Select s with <.value = s.stmt#";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid second attrRef, not a attrName") {
        std::string query = "stmt s; assign a; Select s with s.stmt# = s.val";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid second attrRef, stmt but no #") {
        std::string query = "stmt s; assign a; Select s with s.stmt# = s.stmt";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }

    SECTION("invalid expected =") {
        std::string query = "stmt s; assign a; Select s with s.stmt# <= s.stmt";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(!sv->validateQuery());
    }
}

TEST_CASE("Test and") {
    SECTION("valid such that and") {
        std::string query = "stmt s; assign a; Select s such that Modifies(1,v) and Uses(1,v)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid pattern and") {
        std::string query = "stmt s; assign a; Select s pattern a(_,_) and ifs(a,_,_)";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }

    SECTION("valid with and") {
        std::string query = "stmt s; assign a; Select s with a.stmt# = 1 and \"x\" = 1";
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        PQLTokenScanner pqlTokenScanner(std::move(lexer));
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(pqlTokenScanner);
        requireTrue(sv->validateQuery());
    }
}
