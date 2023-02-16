#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>
#include <memory>

// include your other headers here
#include "AbstractWrapper.h"
#include "pkb/PKB.h"
#include "pkb/PKBWriter.h"
#include "pkb/PKBReader.h"
#include "sp/DesignExtractor.h"
#include "sp/Parser.h"
#include "sp/SourceProcessor.h"

class TestWrapper : public AbstractWrapper {
 public:
    PKB pkb;
    PKBWriter pkbWriter;
    PKBReader pkbReader;
    SourceProcessor sourceProcessor;
    // default constructor
    TestWrapper();

    // destructor
    ~TestWrapper();

    // method for parsing the SIMPLE source
    virtual void parse(std::string filename);

    // method for evaluating a query
    virtual void evaluate(std::string query, std::list<std::string> &results);

    //populate source code from txt files to test
    std::string readFile(std::string filename);
};

#endif
