#include "Result.h"
#include "BoolResult.h"
#include "TableResult.h"

Result::Result(Result::Tag _tag) : tag(_tag) {}

Result* Result::join(Result* lhs, Result* rhs) {
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
    std::unordered_set<std::string> rIdents(r->idents.begin(), r->idents.end());
    for (auto& ident : l->idents) {
      if (rIdents.find(ident) != rIdents.end()) {
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
      // todo join tables
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
