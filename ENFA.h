//
// Created by aaditya on 11.05.22.
//

#ifndef PATTERN_SEARCHING_ENFA_H
#define PATTERN_SEARCHING_ENFA_H
#include "iostream"
#include "string"
#include "map"
#include "fstream"
#include "json.hpp"
#include "set"
using namespace std;
using json = nlohmann::json;

class ENFA {
private:
    json enfa;
    string eps;
    set<vector<string>> allStates;
public:
    explicit ENFA(string p);
    vector<string> tryEpsilon(vector<string> state1);
    bool accept(vector<string> new_state);
    void printStats();
    int transitionCount(string elem);
    int printDegree(int degree);
    bool accepts(string input);
    void nextNodes(vector<int>* node, string input);
    void tryEps(vector<int>* nodes);
};


#endif //PATTERN_SEARCHING_ENFA_H
