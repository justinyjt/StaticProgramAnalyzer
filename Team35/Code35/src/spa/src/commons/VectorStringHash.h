#ifndef SPA_VECTORSTRINGHASH_H
#define SPA_VECTORSTRINGHASH_H

#include <utility>
#include <vector>
#include <string>

struct hash_vector_string {
    std::size_t operator()(std::vector<std::string> const& vec) const {
        std::size_t seed = vec.size();
        for(auto& i : vec) {
            seed ^= std::hash<std::string>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
#endif //SPA_VECTORSTRINGHASH_H
