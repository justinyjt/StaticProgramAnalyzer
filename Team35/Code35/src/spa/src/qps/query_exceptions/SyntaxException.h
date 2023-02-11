#pragma once

#include <exception>

class SyntaxException : public std::exception {
 public:
    [[nodiscard]] const char* what() const noexcept override;
};
