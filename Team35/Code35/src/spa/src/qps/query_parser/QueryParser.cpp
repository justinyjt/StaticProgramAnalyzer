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

std::vector<std::unique_ptr<Clause>> QueryParser::parse(std::string &query) {
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    try {
        // pass tokenList and parse declaration
        std::unique_ptr<DeclarationParser> declarationParser = std::make_unique<DeclarationParser>();
        std::vector<Synonym> synonyms = declarationParser->parse(tokenValidator);
        // parse select using list of found synonyms
        std::unique_ptr<SelectionParser> selectionParser = std::make_unique<SelectionParser>();
        std::unique_ptr<Clause> selectClause = selectionParser->parse(tokenValidator, synonyms);

        std::vector<std::unique_ptr<Clause>> res;
        res.push_back(std::move(selectClause));

        // parse clauses
        std::unique_ptr<ClauseParser> clauseParser = std::make_unique<ClauseParser>();
        std::vector<std::unique_ptr<Clause>> clauses = clauseParser->parse(tokenValidator, synonyms);
        for (auto &clause : clauses) {
            res.push_back(std::move(clause));
        }

        return res;
    } catch (const LexerException &e) {
        throw SyntaxException();
    }
}
