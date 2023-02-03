#include <vector>
#include "qps/clauses/relationships/relationship_clauses/Relationship.h"

class SuchThatClauseParser {
public:
    Relationship parse(Lexer &lexer, std::vector<std::string> synonyms);
};

