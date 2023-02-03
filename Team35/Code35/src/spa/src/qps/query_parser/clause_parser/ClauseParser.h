#include <vector>
#include "qps/query_parser/clause_parser/such_that_clause_parser/SuchThatClauseParser.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"

class ClauseParser {
public:
    std::vector<Clause> parse(Lexer &lexer, std::vector<std::string> synonyms);
private:
    PatternClauseParser patternClauseParser;
    SuchThatClauseParser suchThatClauseParser;
};
