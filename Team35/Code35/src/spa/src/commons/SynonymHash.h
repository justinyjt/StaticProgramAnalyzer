#pragma once

#include <utility>
#include "qps/pql/Synonym.h"

struct hash_synonym {
    size_t operator()(const Synonym& s) const {
        size_t deHash = std::hash<Synonym::DesignEntity>()(s.de);
        size_t identHash = std::hash<std::string>()(s.ident) << 1;
        return deHash ^ identHash;
    }
};
