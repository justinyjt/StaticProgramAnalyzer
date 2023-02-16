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
    enum relationship { MODIFIES, USES, FOLLOWS, FOLLOWS_T, PARENT, PARENT_T };

    Clause* parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms);
    relationship hash(std::string const& string);
    PQLToken* createArg(std::unique_ptr<Token>& token, const std::vector<Synonym>& synonyms);
    Clause* createClause(std::unique_ptr<Token> token1, std::unique_ptr<Token> token2,
                         std::string relationship, std::vector<Synonym> synonyms);
    bool isStmtRef(PQLToken &tok);
    bool isEntRef(PQLToken &tok);
};
