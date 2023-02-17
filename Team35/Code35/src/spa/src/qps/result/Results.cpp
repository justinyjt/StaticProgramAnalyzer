#include "Results.h"

BoolResults::BoolResults(bool b) : b(b) {}

bool BoolResults::operator==(const Results& rhs) const {
    const BoolResults* p_rhs = dynamic_cast<const BoolResults*>(&rhs);
    if (p_rhs != nullptr) {
        return p_rhs != nullptr && b == p_rhs->b;
    }
    return false;
}

TableResults::TableResults(std::vector<std::string> headers, std::vector<std::vector<std::string>> rows) :
    headers(headers), rows(rows) {}

bool TableResults::operator==(const Results& rhs) const {
    const TableResults* p_rhs = dynamic_cast<const TableResults*>(&rhs);
    if (p_rhs != nullptr) {
        return p_rhs != nullptr && headers == p_rhs->headers && rows == p_rhs->rows;
    }
    return false;
}

