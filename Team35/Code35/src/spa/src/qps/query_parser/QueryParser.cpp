#include "QueryParser.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/Lexer.h"

std::pair<Synonym, std::vector<Clause*>> QueryParser::parse(std::string& query) {
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
            Character('(', Token::Tag::LParen),
            Character(')', Token::Tag::RParen),
            Character(',', Token::Tag::Comma),
            Character('\"', Token::Tag::DoubleQuotes),
            Character('_', Token::Tag::Underscore),
            Character(';', Token::Tag::SemiColon),
    };

    std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(query, k, c);

    // pass tokenList and parse declaration
    DeclarationParser declarationParser;
    std::vector<Synonym> synonyms = declarationParser.parse(std::move(lexer));

    // parse select using list of found synonyms
    SelectionParser selectionParser;
    Synonym selectedSynonym = selectionParser.parse(std::move(lexer), synonyms);

    // parse clauses
    ClauseParser clauseParser;
    std::vector<Clause*> clauses = clauseParser.parse(std::move(lexer), synonyms);

    return std::make_pair(selectedSynonym, clauses);
}
