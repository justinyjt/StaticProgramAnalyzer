#pragma once

#include <vector>
#include <string>

class Results {
    virtual bool operator==(const Results &rhs) const = 0;
};

class BoolResults : public Results {
 public:
    explicit BoolResults(bool b);
    bool operator==(const Results& rhs) const;
    bool b;
};

class TableResults : public Results {
 public:
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    TableResults(std::vector<std::string> headers, std::vector<std::vector<std::string>> rows);
    bool operator==(const Results& rhs) const;
};
