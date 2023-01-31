#ifndef INC_22S2_CP_SPA_TEAM_35_RELATIONSHIPTABLE_H
#define INC_22S2_CP_SPA_TEAM_35_RELATIONSHIPTABLE_H

#include <unordered_map>
#include <unordered_set>
#include <utility>

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);

        if (hash1 != hash2) {
            return hash1 ^ hash2;
        }

        // If hash1 == hash2, their XOR is zero.
        return hash1;
    }
};

template<typename TKey, typename TValue>
class RelationshipTable {
    std::unordered_map<TKey, std::unordered_set<TValue>> keyValueMap;
    std::unordered_map<TValue, std::unordered_set<TKey>> valueKeyMap;
    std::unordered_set<std::pair<TKey, TValue>, hash_pair> keyValuePairSet;

    inline static const std::unordered_set<TValue> emptyValSet = std::unordered_set<TValue>();
    inline static const std::unordered_set<TKey> emptyKeySet = std::unordered_set<TKey>();

 public:
    const std::unordered_set<TKey> &getKeys(TValue val) const {
        auto iterator = valueKeyMap.find(val);
        if (iterator == valueKeyMap.end()) {
            return emptyKeySet;
        }
        return iterator->second;
    }

    const std::unordered_set<TValue> &getValues(TKey key) const {
        auto iterator = keyValueMap.find(key);
        if (iterator == keyValueMap.end()) {
            return emptyValSet;
        }
        return iterator->second;
    }

    const std::unordered_set<std::pair<TKey, TValue>> &getKeyValuePairs() const {
        return keyValuePairSet;
    }

    bool containsPair(TKey key, TValue val) const {
        std::pair<TKey, TValue> pair = std::make_pair(key, val);
        return keyValuePairSet.count(pair);
    }

    bool insertPair(TKey key, TValue val) {
        if (containsPair(key, val)) {
            return false;
        }

        keyValueMap[key].emplace(val);
        valueKeyMap[val].emplace(key);
        keyValuePairSet.emplace(std::make_pair(key, val));

        return true;
    }
};
#endif //INC_22S2_CP_SPA_TEAM_35_RELATIONSHIPTABLE_H
