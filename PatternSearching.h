#ifndef PATTERN_SEARCHING_PATTERNSEARCHING_H
#define PATTERN_SEARCHING_PATTERNSEARCHING_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include <string>
#include <utility>
#include <vector>
#include "string"

using namespace std;
using json = nlohmann::json;

class PatternSearching {
private:
    string regex1;
    string regex2;
    string tekstBestand;
    bool UorD = false;

public:
    PatternSearching(string b, string re);
    PatternSearching(string b, string re1, string re2, bool u);
    void search();
};


#endif //PATTERN_SEARCHING_PATTERNSEARCHING_H