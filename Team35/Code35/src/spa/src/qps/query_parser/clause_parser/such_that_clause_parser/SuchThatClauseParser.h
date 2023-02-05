#pragma once

#include <memory>
#include <string>
#include <vector>
#include "qps/clauses/relationships/relationship_clauses/Relationship.h"
#include "qps/entities/Synonym.h"

class SuchThatClauseParser {
 public:
    Relationship parse(std::unique_ptr<Lexer> lexer, std::vector<Synonym> synonyms);
    Term makeTerm(Token token, std::vector<Synonym> synonyms);
};

