#pragma once

#include <exception>
#include <string>

#include "commons/types.h"

class LexerException : public std::exception {
 public:
    LexerException(std::string message, LineNumber line_number);
    [[nodiscard]] const char *what() const noexcept override;

 private:
    std::string message_;
    LineNumber line_number_;
};
