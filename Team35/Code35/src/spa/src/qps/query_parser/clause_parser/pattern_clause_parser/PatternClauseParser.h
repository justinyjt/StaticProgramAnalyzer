#pragma once

#include <string>
#include <memory>
#include <vector>
#include "qps/clauses/patterns/pattern_clauses/Pattern.h"
#include "qps/entities/Synonym.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"

class PatternClauseParser {
 public:
    Pattern parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms);
    Term makeTerm(Token token, std::vector<Synonym> synonyms);
};
