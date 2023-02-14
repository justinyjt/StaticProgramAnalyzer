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
    Tok* createArg(std::unique_ptr<Token>& token, std::vector<Synonym> synonyms);
    Clause* createClause(std::unique_ptr<Token> token1, std::unique_ptr<Token> token2,
                         std::string relationship, std::vector<Synonym> synonyms);
    bool isStmtRef(Tok &tok);
    bool isEntRef(Tok &tok);
};
