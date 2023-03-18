#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <unordered_map>
#include <string>

#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "qps/clause/select_clause/SelectClause.h"

class QueryParser {
 public:
    std::pair<std::unique_ptr<SelectClause>, std::vector<std::unique_ptr<Clause>>> parse(std::string&);
};
