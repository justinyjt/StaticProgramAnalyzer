#include "LexerException.h"

#include <sstream>
#include <utility>

LexerException::LexerException(std::string message, LineNumber line_number) : message_(std::move(message)),
                                                                              line_number_(line_number) {
    std::stringstream s;
    s << "line " << line_number_ << ": " << "lexical error: " << message_;
    message_ = s.str();
}

const char *LexerException::what() const noexcept {
    return message_.c_str();
}
