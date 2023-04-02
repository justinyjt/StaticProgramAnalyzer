#pragma once

#include <unordered_set>

template<typename TEntity>
class EntityTable {
    std::unordered_set<TEntity> entitySet;
 public:
    const std::unordered_set<TEntity> &getTable() const {
        return entitySet;
    }

    bool hasEntity(TEntity entity) const {
        return entitySet.count(entity);
    }

    void addEntity(TEntity entity) {
        entitySet.emplace(entity);
    }
};
