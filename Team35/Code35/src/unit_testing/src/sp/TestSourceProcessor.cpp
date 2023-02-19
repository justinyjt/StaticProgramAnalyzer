#include <memory>
#include <string>

#include "../TestHelper.h"
#include "sp/SourceProcessor.h"
#include "pkb/PKBReader.h"
#include "pkb/PKB.h"
#include "catch.hpp"

TEST_CASE("SourceProcessor can handle valid source code") {
    PKB pkb;
    SourceProcessor source_processor(std::make_unique<PKBWriter>(pkb));
    std::string source = "procedure Example {\n"
                         "  x = 2;\n"
                         "  z = 3;\n"
                         "  i = 5;\n"
                         "  while ((i!=0)||(!(x==1))) {\n"
                         "    x = x - 1;\n"
                         "    if (x<=1) then {\n"
                         "      z = x + 1; }\n"
                         "    else {\n"
                         "      y = z + x; }\n"
                         "    z = z + x + i;\n"
                         "    i = i - 1; }\n"
                         "  }\n"
                         "\n"
                         "procedure p {\n"
                         "  if (x<0) then {\n"
                         "    while (i>0) {\n"
                         "      x = z * 3 + 2 * y;\n"
                         "      i = i - 1; }\n"
                         "    print z;\n"
                         "    z = x + z; }\n"
                         "  else {\n"
                         "    read v; }\n"
                         "  z = z + x + i; }\n"
                         "\n"
                         "procedure q {\n"
                         "  if (x>=1) then {\n"
                         "    z = x + 1; }\n"
                         "  else {\n"
                         "    x = z + x; } }";
    source_processor.process(source);
}

TEST_CASE("SourceProcessor can handle invalid source code") {
    PKB pkb;
    SourceProcessor source_processor(std::make_unique<PKBWriter>(pkb));
    std::string source = "procedure Example {\n"
                         "  x = 2;\n"
                         "  z = 3;\n"
                         "  i = 5;\n"
                         "  while ((i-1)||(x<2)) {\n"
                         "    x = x - 1;\n"
                         "    if (x==1) then {\n"
                         "      z = x + 1; }\n"
                         "    else {\n"
                         "      y = z + x; }\n"
                         "    z = z + x + i;\n"
                         "    i = i - 1; }\n"
                         "  }\n"
                         "\n"
                         "procedure p {\n"
                         "  if (x<0) then {\n"
                         "    while (i>0) {\n"
                         "      x = z * 3 + 2 * y;\n"
                         "      i = i - 1; }\n"
                         "    x = x + 1;\n"
                         "    z = x + z; }\n"
                         "  else {\n"
                         "    z = 1; }\n"
                         "  z = z + x + i; }\n"
                         "\n"
                         "procedure q {\n"
                         "  if (x==1) then {\n"
                         "    z = x + 1; }\n"
                         "  else {\n"
                         "    x = z + x; } }";
    source_processor.process(source);
}
