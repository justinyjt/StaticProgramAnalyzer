#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/clause/relationship/Relationship.h"
#include "qps/pql/Synonym.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

class SuchThatClauseParser {
 public:
    Clause *parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms);
    PQLToken* makeArg(std::unique_ptr<Token> token, std::vector<Synonym> synonyms);
};
