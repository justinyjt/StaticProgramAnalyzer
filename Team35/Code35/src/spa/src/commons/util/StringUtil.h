#pragma once

#include <string>

#include "commons/types.h"

struct StringUtil {
    inline static std::string toString(STMT_NUM stmtNum) {
        return std::to_string(stmtNum);
    }

    inline static std::string toString(ENT_NAME entName) {
        return entName;
    }
};
