#pragma once

#include <stdexcept>

#include "SemanticValidator.h"
#include "qps/query_exceptions/SemanticException.h"

bool SemanticValidator::isDeclared(std::unordered_map<std::string, Synonym::DesignEntity>& declarations,
                                   std::string synonym) {
    return declarations.find(synonym) != declarations.end();
}

Synonym::DesignEntity SemanticValidator::getDesignEntity(std::unordered_map<std::string,
                                                         Synonym::DesignEntity>& declarations, std::string synonym) {
    try {
        return declarations.at(synonym);
    } catch (const std::out_of_range &e) {
        throw SemanticException();
    }
}
