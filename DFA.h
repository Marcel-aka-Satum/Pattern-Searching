#ifndef SUBSET_CONSTRUCTION_DFA_H
#define SUBSET_CONSTRUCTION_DFA_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;
using namespace std;
class DFA {
private:
    json j;
    json dfa1;
    json dfa2;
    bool unieOrDoorsnede;
public:
    //common functions
    DFA(const string& dfa);
    void print();
    bool accepts(const string& s);

    //functions for product automaat

    DFA(json v, bool test);
    DFA(const DFA& a, const DFA& b, bool c);
    void doUnie();
    void doDoorsnede();

    //functions for mssc

    //DFA(json v);
};

#endif //SUBSET_CONSTRUCTION_DFA_H
