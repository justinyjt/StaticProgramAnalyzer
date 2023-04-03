#pragma once

#include <cstdint>
#include <queue>
#include <unordered_set>
#include <vector>

typedef uint32_t ClauseIndex;
typedef std::vector<ClauseIndex> ClauseIndexList;
typedef std::unordered_set<ClauseIndex> ClauseIndexSet;
typedef std::queue<ClauseIndex> ClauseIndexQueue;
