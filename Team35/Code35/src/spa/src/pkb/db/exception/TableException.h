#pragma once

#include <exception>

class TableException : public std::exception {
 public:
    [[nodiscard]] const char *what() const noexcept override;
};
