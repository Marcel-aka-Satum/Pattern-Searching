//
// Created by aaditya on 23.04.22.
//

#ifndef SUBSET_CONSTRUCTION_DFA_H
#define SUBSET_CONSTRUCTION_DFA_H
#include "iostream"
#include "fstream"
#include "string"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class DFA {
private:
    json j;
public:

    DFA(string file);

    void print();

};

#endif //SUBSET_CONSTRUCTION_DFA_H
