#include "LexerException.h"

#include <sstream>
#include <utility>

LexerException::LexerException(std::string message, LineNumber line_number) : message_(std::move(message)),
                                                                              line_number_(line_number) {}

const char *LexerException::what() const noexcept {
    std::stringstream s;
    s << "line " << line_number_ << ": " << "lexical error: " << message_;
    return s.str().c_str();
}
