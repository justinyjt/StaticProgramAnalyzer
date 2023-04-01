#pragma once

#include <exception>
#include <string>

class LexerException : public std::exception {
 public:
    LexerException(std::string message);
    [[nodiscard]] const char *what() const noexcept override;

 private:
    std::string message_;
};
