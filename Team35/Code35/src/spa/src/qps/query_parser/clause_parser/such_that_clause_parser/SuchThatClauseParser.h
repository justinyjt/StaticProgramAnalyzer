#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/pql/Synonym.h"
#include "qps/clause/Clause.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

class SuchThatClauseParser {
 public:
    std::unique_ptr<Clause> parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms);
    std::shared_ptr<PQLToken> createArg(std::unique_ptr<Token>& token, const std::vector<Synonym>& synonyms);
    std::unique_ptr<Clause> createClause(std::unique_ptr<Token> token1, std::unique_ptr<Token> token2,
                         std::string relationship, std::vector<Synonym> synonyms);
    bool isStmtRef(PQLToken &tok);
    bool isEntRef(PQLToken &tok);
};
