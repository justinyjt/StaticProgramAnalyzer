#pragma once

#include <exception>

class SemanticException : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override;
};
