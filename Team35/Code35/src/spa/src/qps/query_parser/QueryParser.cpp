#include "QueryParser.h"
#include "qps/clause/SelectClause.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/lexer/ILexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "commons/lexer/exception/LexerException.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "commons/token_scanner/PQLTokenScanner.h"

std::vector<std::unique_ptr<Clause>> QueryParser::parse(std::string &query) {
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    try {
        // pass tokenList and parse declaration
        std::vector<Synonym> declarationList;
        DeclarationParser declarationParser(pqlTokenScanner, declarationList);
        std::vector<Synonym> synonyms = declarationParser.parse();
        // parse select using list of found synonyms
        SelectionParser selectionParser(pqlTokenScanner, synonyms);
        std::unique_ptr<Clause> selectClause = selectionParser.parse();

        std::vector<std::unique_ptr<Clause>> res;
        res.push_back(std::move(selectClause));

        // parse clauses
        ClauseParser clauseParser(pqlTokenScanner, synonyms);
        std::vector<std::unique_ptr<Clause>> clauses = clauseParser.parse();
        for (auto &clause : clauses) {
            res.push_back(std::move(clause));
        }

        return res;
    } catch (const LexerException &e) {
        throw SyntaxException();
    }
}
