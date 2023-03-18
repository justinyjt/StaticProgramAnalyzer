#include "QueryParser.h"
#include "qps/clause/select_clause/SelectClause.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/exception/LexerException.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "commons/token_scanner/PQLTokenScanner.h"
#include "QuerySyntaxValidator.h"

std::pair<std::unique_ptr<SelectClause>, std::vector<std::unique_ptr<Clause>>> QueryParser::parse(std::string &query) {
    try {
        std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
        std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(std::move(lexer));
        if (!sv->validateQuery()) {
            throw SyntaxException();
        }
        PQLTokenScanner pqlTokenScanner(std::move(sv->getTokenLst()));
        // pass tokenList and parse declaration
        std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
        // std::vector<Synonym> declarationList;
        DeclarationParser declarationParser(pqlTokenScanner, declarationList);
        std::unordered_map<std::string, Synonym::DesignEntity> synonyms = declarationParser.parse();
        // parse select using list of found synonyms
        SelectionParser selectionParser(pqlTokenScanner, synonyms);
        std::unique_ptr<SelectClause> selectClause = selectionParser.parse();

        // parse clauses
        ClauseParser clauseParser(pqlTokenScanner, synonyms);
        std::vector<std::unique_ptr<Clause>> clauses = clauseParser.parse();

        return {std::move(selectClause), std::move(clauses)};
    } catch (const LexerException &e) {
        throw SyntaxException();
    }
}
