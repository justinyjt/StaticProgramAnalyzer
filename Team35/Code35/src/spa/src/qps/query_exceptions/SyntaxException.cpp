#include "SyntaxException.h"

const char *SyntaxException::what() const noexcept {
    return "Syntax error";
}
