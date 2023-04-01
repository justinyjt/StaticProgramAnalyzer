#include "TableException.h"

const char *TableException::what() const noexcept {
    return "Access error, table does not exist";
}
