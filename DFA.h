//
// Created by aaditya on 23.04.22.
//

#ifndef SUBSET_CONSTRUCTION_DFA_H
#define SUBSET_CONSTRUCTION_DFA_H
#include <string>
#include "iomanip"
#include <iostream>
#include "json.hpp"
#include <set>
#include <map>

using json = nlohmann::json;
using namespace std;
class DFA {
private:
    string filename;
    json j;
    json dfa1;
    json dfa2;
    bool doorsnede;
public:

    DFA(string p);

    void print();

};

#endif //SUBSET_CONSTRUCTION_DFA_H
