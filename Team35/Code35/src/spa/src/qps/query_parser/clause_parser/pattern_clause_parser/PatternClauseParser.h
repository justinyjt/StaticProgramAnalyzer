#include <vector>
#include "qps/clauses/patterns/pattern_clauses/Pattern.h"

class PatternClauseParser {
public:
    Pattern parse(Lexer &lexer, std::vector<std::string> synonyms);
};
