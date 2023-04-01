#include "LexerException.h"

#include <sstream>
#include <utility>

LexerException::LexerException(std::string message) : message_(std::move(message)) {
    std::stringstream s;
    s << "lexical error: " << message_;
    message_ = s.str();
}

const char *LexerException::what() const noexcept {
    return message_.c_str();
}
