#include "QPS.h"
#include <memory>

QPS::QPS(std::list<std::string>& results) : results(results), queryParser(new QueryParser()) {}

void QPS::executeQuery(std::string* queryString) {
    // Create lexer for queryString
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
            Character("(", Tag::Lparen),
            Character(")", Tag::RParen),
            Character(",", Tag::Comma),
            Character("\"", Tag::DoubleQuotes),
            Character("_", Tag::Underscore),
            Character(";", Tag::SemiColon),
    };

    std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(*queryString, k, c);

    // Perform parsing
    queryParser->setLexer(std::move(lexer));
    std::pair<std::string, std::vector<Clause>> parseResult = queryParser->parse();

    // Perform evaluation
}