#ifndef PATTERN_SEARCHING_ENFA_H
#define PATTERN_SEARCHING_ENFA_H
#include "iostream"
#include "DFA.h"
#include "unordered_set"
#include "vector"
#include "algorithm"
#include "queue"
#include "iomanip"
#include "string"
#include "map"
#include "fstream"
#include "json.hpp"
#include "set"
using namespace std;
using json = nlohmann::json;

class ENFA {
private:
    json j;
    string eps;
    set<vector<string>> allStates;
public:
    //common function
    ENFA(const string& p);
    //functions for regex to enfa
    vector<string> tryEpsilon(vector<string> state1);
    bool accept(vector<string> new_state);
    void printStats();
    int transitionCount(string elem);
    int printDegree(int degree);
    bool accepts(string input);
    void nextNodes(vector<int>* node, string input);
    void tryEps(vector<int>* nodes);

    //functions for mssc

    bool accept(const string& s);
    void print();
    DFA toDFA();

};


#endif //PATTERN_SEARCHING_ENFA_H
