#pragma once

#include <unordered_set>

class SetUtil {
 public:
    template<typename T>
    static std::unordered_set<T> intersection(const std::unordered_set<T> &set1, const std::unordered_set<T> &set2) {
        std::unordered_set<T> result;
        for (const auto &element : set1) {
            if (set2.find(element) != set2.end()) {
                result.insert(element);
            }
        }
        return result;
    }

    template<typename T>
    static bool hasIntersection(const std::unordered_set<T> &set1, const std::unordered_set<T> &set2) {
        for (const auto &element : set1) {
            if (set2.find(element) != set2.end()) {
                return true;
            }
        }
        return false;
    }
};
