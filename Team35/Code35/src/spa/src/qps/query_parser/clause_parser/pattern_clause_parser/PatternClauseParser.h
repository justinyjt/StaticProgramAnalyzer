#pragma once

#include <string>
#include <memory>
#include <vector>
#include <utility>

#include "qps/clause/Clause.h"
#include "qps/clause/Pattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

class PatternClauseParser {
 public:
    std::unique_ptr<Clause> parse(TokenValidator &tokenValidator, std::vector<Synonym>& synonyms);
    std::unique_ptr<PQLToken> createLeftArg(std::unique_ptr<Token>& token, const std::vector<Synonym>& synonyms);
    std::unique_ptr<PQLToken> createRightArg(std::vector<std::unique_ptr<Token>> &tokenList);
    std::unique_ptr<Clause> createClause(std::unique_ptr<Token>& token1, std::vector<std::unique_ptr<Token>>& token2,
                                              std::vector<Synonym>& synonyms, const std::string& patternStr);
    bool isValidPatternSynonym(const std::string& synonym, std::vector<Synonym>& synonyms);
    bool isEntRef(PQLToken &tok);
};
