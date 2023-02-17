#pragma once
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "qps/result/BoolResult.h"
#include "qps/result/TableResult.h"
#include "commons/types.h"

/*
represents result of single clause evaluation 
or query evaluation (multiple clause evaluations)
*/

// Select st s.t. Modifies(1, v) pattern a(y, "x + 1")

class Result {
 public:
    enum class Tag { BOOL, TABLE };
    Tag tag;
    explicit Result(Tag _tag) : tag(_tag) {}
    virtual void output(std::list<std::string>&) = 0;
    static Result* join(Result* lhs, Result* rhs) {

      // case bool <-> bool:
      if (lhs->tag == Tag::BOOL && rhs->tag == Tag::BOOL) {
        BoolResult* l = dynamic_cast<BoolResult*>(lhs);
        BoolResult* r = dynamic_cast<BoolResult*>(rhs);
        Result* result = new BoolResult(l->b && r->b);
      }

      // case table <-> table:
      if (lhs->tag == Tag::TABLE && rhs->tag == Tag::TABLE) {
        TableResult* l = dynamic_cast<TableResult*>(lhs);
        TableResult* r = dynamic_cast<TableResult*>(rhs);
        // assuming only maximum 1 column overlap
        std::string commonIdent("");
        for (auto& ident : l->idents) {
          if (r->idents.find(ident) != r->idents.end()) {
            commonIdent = ident;
          }
        }

        if (commonIdent != "") {
          // get stricter set
          Result* larger; 
          Result* smaller; 
          if (l->rows.size() > r->rows.size())  {
            larger = l;
            smaller = r;
          } else {
            larger = r;
            smaller = l;
          }
          // TODO join tables
          // <x,a> <x>
          // <1,2> <5>
          // <3,4> <6>
          // <5,6> <7>
          
          return nullptr;
        } 

        // no column to join on, return empty set???
        Result* result = new TableResult();
        return result;
      }
      
      // case table <-> bool or bool <-> table:
      BoolResult* br = dynamic_cast<BoolResult*>(rhs);
      TableResult* tr = dynamic_cast<TableResult*>(rhs);
      Result* result;

      if (!br->b) {  // empty set
        std::vector<std::list<std::string>> rows;
        result = new TableResult(tr->idents, rows);
      } else {  // make a copy
        result = new TableResult(*tr);
      }

      return result;
    }
};