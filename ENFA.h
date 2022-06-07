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
    ENFA* __initCheck;
    json j;
    json dfa;
    string eps;
    set<vector<string>> allStates;
public:
    //common function
    //ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
    ENFA();
    //ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
    ENFA(const string& p);

    const string &getEps() const;

    int getAllStates() const;

    //functions for regex to enfa
    vector<string> tryEpsilon(vector<string> state1);
    bool accept(vector<string> new_state);
    bool accepts(string input);
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling nextNodes");
    void nextNodes(vector<string>* node, string input);
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling tryEps");
    void tryEps(vector<string>* nodes);
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling subsetConstruction");
    void subsetConstruction(vector<string> const &state);
    static string vecToString(vector<string> new_state);
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling findTransition");
    vector<string> findTransition(vector<string> state, string input);
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling addTransition");
    void addTransition(string from, string to, string input);
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling addState");
    void addState(string name, bool starting, bool accepting);


    //functions for mssc
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling accept");
    bool accept(const string& s);
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling print");
    void print();
    //REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling toDFA");
    DFA toDFA();

    bool properlyInitialized() const;

};


#endif //PATTERN_SEARCHING_ENFA_H
