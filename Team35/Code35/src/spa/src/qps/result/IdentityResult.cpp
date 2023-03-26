#include "IdentityResult.h"

std::unique_ptr<Result> IdentityResult::join(Result &rhs) {
    switch (rhs.tag) {
        case Result::Tag::TABLE: {
            TableResult t = dynamic_cast<TableResult &>(rhs);
            return std::make_unique<TableResult>(t);
        }
        case Result::Tag::BOOL: {
            BoolResult b = dynamic_cast<BoolResult&>(rhs);
            return std::make_unique<BoolResult>(b);
        }
    }
}

void IdentityResult::output(std::list<std::string>& list) {
}


