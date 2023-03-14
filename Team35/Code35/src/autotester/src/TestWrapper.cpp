#include <fstream>
#include <utility>

#include "TestWrapper.h"
#include "qps/QPS.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_exceptions/SemanticException.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = 0;

AbstractWrapper *WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}

// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() : pkbWriter(PKBWriter(pkb)),
                             pkbReader(PKBReader(pkb)),
                             sourceProcessor(SourceProcessor(std::make_unique<PKBWriter>(pkb))) {
    // create any objects here as instance variables of this class
    // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
std::string TestWrapper::readFile(std::string filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cout << "File path provided is incorrect.";
        exit(1);
    }
    std::string simpleProgram((std::istreambuf_iterator<char>(inputFile)),
                              std::istreambuf_iterator<char>());
    return simpleProgram;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    try {
        std::cout << "successful call" << std::endl;
        std::string simpleProgramCode = readFile(filename);
        bool isProcessSuccess = sourceProcessor.process(simpleProgramCode);
        if (!isProcessSuccess) {
            throw;
        }
        // INCLUDE SourceProcessor's method here to parse, extract knowledge and store info into pkb
    } catch (std::exception &e) {
        std::cout << e.what();
    }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
    // QPS.evaluate(query, results);
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...
    try {
        QPS queryProcessor = QPS(&pkbReader);
        std::string &s = query;
        queryProcessor.executeQuery(s, results);
    } catch (std::exception &e) {
        std::cout << e.what();
    }


    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
}
