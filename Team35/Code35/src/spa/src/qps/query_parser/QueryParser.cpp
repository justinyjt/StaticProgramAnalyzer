#include "QueryParser.h"
#include "qps/clause/SelectClause.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/lexer/ILexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

std::vector<Clause*> QueryParser::parse(std::string& query) {
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    // pass tokenList and parse declaration
    DeclarationParser declarationParser;
    std::vector<Synonym> synonyms = declarationParser.parse(tokenValidator);

    // parse select using list of found synonyms
    SelectionParser selectionParser;
    Synonym selectedSynonym = selectionParser.parse(tokenValidator, synonyms);
    Clause* selectClause = new SelectClause(selectedSynonym);

    std::vector<Clause*> res{selectClause};

    // parse clauses
    ClauseParser clauseParser;
    std::vector<Clause*> clauses = clauseParser.parse(tokenValidator, synonyms);
    res.insert(res.end(), clauses.begin(), clauses.end());

    return res;
}
