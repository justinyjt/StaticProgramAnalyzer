#include "SemanticException.h"

const char *SemanticException::what() const noexcept {
    return "SemanticError";
}
