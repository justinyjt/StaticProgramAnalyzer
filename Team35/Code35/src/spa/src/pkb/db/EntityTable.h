#ifndef TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_ENTITYTABLE_H_
#define TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_ENTITYTABLE_H_
#include <unordered_set>

template <typename TEntity>
class EntityTable {
    std::unordered_set<TEntity> entitySet;
 public:
    const std::unordered_set<TEntity> &getTable() const {
        return entitySet;
    }

    bool hasEntity(TEntity entity) const {
        return entitySet.count(entity);
    }

    bool addEntity(TEntity entity) {
        auto res = entitySet.emplace(entity);
        return res.second;
    }
};


#endif  // TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_ENTITYTABLE_H_
