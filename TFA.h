//
// Created by Yassine on 22/05/2022.
//

#ifndef PATTERN_SEARCHING_TFA_H
#define PATTERN_SEARCHING_TFA_H

#include "json.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;
using json = nlohmann::json;


class TFA {
private:
    json j;
    json dfa;
    vector<string> names;
    vector<string> names2;
    vector<string> checker;
    vector<string> checkerMini;
    vector<string> over;
    vector<string> rdy;

public:
    TFA(string dfa);

    void printTable();
    TFA minimize();
    void print();
};



#endif //PATTERN_SEARCHING_TFA_H
