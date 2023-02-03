#include "QueryParser.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"

void QueryParser::setLexer(Lexer lexer) {
    this->lexer = lexer;
}

std::pair<std::string, std::vector<Clause>> QueryParser::parse() {

    // pass tokenList and parse declaration
    DeclarationParser declarationParser;
    std::vector<std::string> synonyms = declarationParser.parse(lexer);

    // parse select using list of found synonyms
    SelectionParser selectionParser;
    std::string selectedSynonym = selectionParser.parse(lexer, synonyms);

    // parse clauses
    ClauseParser clauseParser;
    std::vector<Clause> clauses = clauseParser.parse(lexer, synonyms);

    return std::make_pair(selectedSynonym, clauses);
};