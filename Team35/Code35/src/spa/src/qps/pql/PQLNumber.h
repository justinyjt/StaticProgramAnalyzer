#pragma once

#include <string>
#include "PQLToken.h"

class PQLNumber : public PQLToken {
 public:
    explicit PQLNumber(std::string n);

    std::string n;

    std::string str() const override;

    bool operator==(const PQLToken &rhs) const override;
};
