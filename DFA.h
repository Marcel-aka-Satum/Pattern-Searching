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
    bool doorsnede_unie;
    bool unieOrDoorsnede;
    //TFA variable
    json dfa;
    vector<string> names;
    vector<string> names2;
    vector<string> checker;
    vector<string> checkerMini;
    vector<string> over;
    vector<string> rdy;
public:
    //common functions
    DFA(const string& dfa);
    void print();
    bool accepts(const string& s);

    //functions for product automaat

    DFA(json v, bool test);
    DFA(const DFA& a, const DFA& b, bool c);
    DFA productAutomaat();
    void doUnie();
    void doDoorsnede();


    //function tfa
    void printTable();
    DFA minimize();
};

#endif //SUBSET_CONSTRUCTION_DFA_H
