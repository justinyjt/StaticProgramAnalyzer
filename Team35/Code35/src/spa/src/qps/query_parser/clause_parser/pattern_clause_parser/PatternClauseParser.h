#pragma once

#include <string>
#include <memory>
#include <vector>
#include "qps/clauses/patterns/pattern_clauses/Pattern.h"
#include "qps/entities/Synonym.h"

class PatternClauseParser {
 public:
    Pattern parse(std::unique_ptr<Lexer> lexer, std::vector<std::string> synonyms);
    Term makeTerm(Token token, std::vector<Synonym> synonyms);
};
