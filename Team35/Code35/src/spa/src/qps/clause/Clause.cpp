#include "Clause.h"

#include <utility>

bool Clause::operator!=(const Clause &rhs) const {
    return !(*this == rhs);
}

uint32_t Clause::getHeaderCount() const {
    return this->headers_.size();
}

const HEADER_SET &Clause::getHeaders() const {
    return this->headers_;
}

void Clause::addHeader(HEADER header) {
    this->headers_.insert(std::move(header));
}

void Clause::addHeaders(const HEADER_SET &headers) {
    this->headers_.insert(headers.begin(), headers.end());
}
