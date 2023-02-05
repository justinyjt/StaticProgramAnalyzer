#pragma once

#include <memory>
#include <string>
#include <vector>
#include "qps/clause/relationship/Relationship.h"
#include "qps/clause/Arg.h"
#include "qps/pql/Synonym.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"

class SuchThatClauseParser {
 public:
    Clause& parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms);
    Arg makeArg(Token token, std::vector<Synonym> synonyms);
};
