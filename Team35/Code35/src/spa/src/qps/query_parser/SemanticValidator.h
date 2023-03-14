#pragma once

#include <string>
#include <unordered_map>

#include "qps/pql/Synonym.h"

class SemanticValidator {
 public:
    static bool isDeclared(std::unordered_map<std::string, Synonym::DesignEntity>& declarations, std::string synonym);
    static Synonym::DesignEntity getDesignEntity(std::unordered_map<std::string, Synonym::DesignEntity>& declarations,
                                                 std::string synonym);
};
