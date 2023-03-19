#pragma once

#include <string>

#include "PQLToken.h"
#include "Synonym.h"

class AttrRef : public Synonym {
 public:
    enum class AttrVal {
        NAME, INTEGER
    };

    AttrRef(DesignEntity de, std::string ident, std::string ref);

    bool operator==(const PQLToken& rhs) const override;
    std::string str() const override;

    const std::string ref;
};
