#include "QueryParser.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/Lexer.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/lexer/ILexer.h"

std::pair<Synonym, std::vector<Clause*>> QueryParser::parse(std::string& query) {

    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    // pass tokenList and parse declaration
    DeclarationParser declarationParser;
    std::vector<Synonym> synonyms = declarationParser.parse(lexer);

    // parse select using list of found synonyms
    SelectionParser selectionParser;
    Synonym selectedSynonym = selectionParser.parse(std::move(lexer), synonyms);

    // parse clauses
    ClauseParser clauseParser;
    std::vector<Clause*> clauses = clauseParser.parse(lexer, synonyms);

    return std::make_pair(selectedSynonym, clauses);
}
