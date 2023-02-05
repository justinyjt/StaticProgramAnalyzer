#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"

class QueryParser {
 public:
    QueryParser(const std::unique_ptr<Lexer> &lexer);
    std::pair<Synonym, std::vector<Clause>> parse();
 private:
    std::unique_ptr<Lexer> &lexer_;
};
