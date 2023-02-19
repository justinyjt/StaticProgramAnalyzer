#ifndef TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_RELATIONSHIPTABLE_H_
#define TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_RELATIONSHIPTABLE_H_

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "commons/PairHash.h"

template<typename TKey, typename TValue>
class RelationshipTable {
    std::unordered_map<TKey, std::unordered_set<TValue>> keyValueMap;
    std::unordered_map<TValue, std::unordered_set<TKey>> valueKeyMap;
    std::unordered_set<std::pair<TKey, TValue>, hash_pair> keyValuePairSet;

    inline static const std::unordered_set<TValue> emptyValSet =
            std::unordered_set<TValue>();
    inline static const std::unordered_set<TKey> emptyKeySet =
            std::unordered_set<TKey>();

 public:
    const std::unordered_set<TKey> getKeys() const {
        std::unordered_set<TKey> res;
        for (auto const& keys : keyValueMap) {
            res.emplace(keys.first);
        }
        return res;
    }

    const std::unordered_set<TValue> getValues() const {
        std::unordered_set<TValue> res;
        for (auto const& values : valueKeyMap) {
            res.emplace(values.first);
        }
        return res;
    }

    const std::unordered_set<TKey> &getKeysByValue(TValue val) const {
        auto iterator = valueKeyMap.find(val);
        if (iterator == valueKeyMap.end()) {
            return emptyKeySet;
        }
        return iterator->second;
    }

    const std::unordered_set<TValue> &getValuesByKey(TKey key) const {
        auto iterator = keyValueMap.find(key);
        if (iterator == keyValueMap.end()) {
            return emptyValSet;
        }
        return iterator->second;
    }

    const std::unordered_set<std::pair<TKey, TValue>, hash_pair> &getKeyValuePairs() const {
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
#endif  // TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_RELATIONSHIPTABLE_H_
