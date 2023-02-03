#ifndef TEAM35_CODE35_SRC_SPA_SRC_COMMONS_PAIRHASH_H_
#define TEAM35_CODE35_SRC_SPA_SRC_COMMONS_PAIRHASH_H_

#include <utility>

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);

        if (hash1 != hash2) {
            return hash1 ^ hash2;
        }

        // If hash1 == hash2, their XOR is zero.
        return hash1;
    }
};

#endif  // TEAM35_CODE35_SRC_SPA_SRC_COMMONS_PAIRHASH_H_
