#include "QueryParser.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"

void QueryParser::setLexer(std::unique_ptr<Lexer> l) {
    lexer_ = std::move(l);
}

std::pair<Synonym, std::vector<Clause>> QueryParser::parse() {
    // pass tokenList and parse declaration
    DeclarationParser declarationParser;
    std::vector<Synonym> synonyms = declarationParser.parse(std::move(lexer_));

    // parse select using list of found synonyms
    SelectionParser selectionParser;
    Synonym selectedSynonym = selectionParser.parse(std::move(lexer_), synonyms);

    // parse clauses
    ClauseParser clauseParser;
    std::vector<Clause> clauses = clauseParser.parse(std::move(lexer_), synonyms);

    return std::make_pair(selectedSynonym, clauses);
}
