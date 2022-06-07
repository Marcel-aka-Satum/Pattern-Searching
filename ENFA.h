#ifndef PATTERN_SEARCHING_ENFA_H
#define PATTERN_SEARCHING_ENFA_H
#include "iostream"
#include "DFA.h"
#include "unordered_set"
#include "vector"
#include "contract.h"
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
    json dfa;
    string eps;
    set<vector<string>> allStates;
public:
    //common function
    ENFA();

    ENFA(const string& p);

    const string &getEps() const;

    int getAllStates() const;

    //functions for regex to enfa
    vector<string> tryEpsilon(vector<string> state1);
    bool accept(vector<string> new_state);
    void printStats();
    int transitionCount(string elem);
    int printDegree(int degree);
    bool accepts(string input);
    void nextNodes(vector<string>* node, string input);
    void tryEps(vector<string>* nodes);
    void subsetConstruction(vector<string> const &state);
    static string vecToString(vector<string> new_state);
    vector<string> findTransition(vector<string> state, string input);
    void addTransition(string from, string to, string input);
    void addState(string name, bool starting, bool accepting);


    //functions for mssc

    bool accept(const string& s);
    void print();
    DFA toDFA();

};


#endif //PATTERN_SEARCHING_ENFA_H
