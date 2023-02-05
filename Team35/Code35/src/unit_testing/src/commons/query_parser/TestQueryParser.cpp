#include "catch.hpp"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/query_parser/QueryParser.h"
#include "../../TestHelper.h"

TEST_CASE("1. Lexer test basic token") {
    KeywordList k {
            Keyword("Select"),
            Keyword("such"),
            Keyword("that"),
            Keyword("Modifies"),
            Keyword("pattern"),
            Keyword("variable"),
            Keyword("assign"),
    };

    CharacterList c {
            Character('(', Token::Tag::LParen),
            Character(')', Token::Tag::RParen),
            Character(',', Token::Tag::Comma),
            Character('\"', Token::Tag::DoubleQuotes),
            Character('_', Token::Tag::Underscore),
            Character(';', Token::Tag::SemiColon),
    };

    std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>("Select s such that Modifies(1, v)\n Select a pattern a ( _ , \"x + 1\")", k, c);

    QueryParser queryParser = QueryParser();

    // Perform parsing
    queryParser.setLexer(std::move(lexer));
    std::pair<Synonym, std::vector<Clause>> parseResult = queryParser.parse();

    Synonym synonym = Synonym(Synonym::DesignEntity::Variable, "s");
    RequireEqual(Synonym(Synonym::DesignEntity::Variable, "s"), parseResult.first);
}