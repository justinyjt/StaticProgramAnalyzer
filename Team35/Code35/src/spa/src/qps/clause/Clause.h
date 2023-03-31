#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "qps/result/Result.h"
#include "qps/result/TableResult.h"
#include "qps/result/BoolResult.h"
#include "pkb/PKBReader.h"
#include "commons/types.h"

/* 
Base class for clause in PQL (suchthat-cl, pattern-cl)
                             Clause
                                |
                                |
                /                                    \
               /                               OptimisableClause
              /                 |                                         \
       select_clause       OneArgClause                               TwoArgClause
       /    |    \          /        \                   /           |              |           \
Boolean Single Tuple  IfPattern   WhilePattern   AssignPattern StmtNameClause StmtStmtClause EntEntClause
                                                                     |              |            |
                                                                Modifies/Uses Parent/Follows   Calls
*/

typedef std::string HEADER;
typedef std::unordered_set<HEADER> HEADER_SET;

class Clause {
 public:
    virtual bool operator==(const Clause &rhs) const = 0;
    bool operator!=(const Clause &rhs) const;
    Clause() = default;
    virtual ~Clause() = default;
    Clause(const Clause &) = delete;
    Clause(Clause &&) = delete;
    Clause &operator=(const Clause &) = delete;
    Clause &operator=(Clause &&) = delete;

    virtual std::unique_ptr<Result> evaluate(PKBReader *db) = 0;

    uint32_t getHeaderCount() const;
    const HEADER_SET &getHeaders() const;

 protected:
    void addHeader(HEADER header);
    void addHeaders(const HEADER_SET &headers);

 private:
    HEADER_SET headers_;
};
