//
// Created by aaditya on 12.04.22.
//

#ifndef SUBSET_CONSTRUCTION_NFA_H
#define SUBSET_CONSTRUCTION_NFA_H
#include "iostream"
#include "iomanip"
#include "algorithm"
#include "DFA.h"
#include "set"
#include "fstream"
#include "vector"
#include "string"
#include "json.hpp"
class DFA;
using namespace std;
using json = nlohmann::json;
class NFA {
private:
    json k;
    json d;
    set<vector<string>> new_states;
public:
    NFA(string filename);

    void addStates(string name, bool starting, bool accepting);

    void addTransition(string from, string to, string input);

    void vind_states(vector<string>const  &state);

    bool accept(vector<string> state);

    vector<string> vind_transities(vector<string> state, string input);

    string vecTostring(vector<string> states);

     DFA toDFA();

};


#endif //SUBSET_CONSTRUCTION_NFA_H
