#pragma once

#include <string>
#include <memory>
#include <vector>
#include "qps/clause/pattern/Pattern.h"
#include "qps/clause/Arg.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"

class PatternClauseParser {
 public:
    Clause& parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms);
};
