#ifndef SUBSET_CONSTRUCTION_DFA_H
#define SUBSET_CONSTRUCTION_DFA_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include "contract.h"
#include <string>
#include <utility>
#include <vector>
using json = nlohmann::json;
using namespace std;
struct State{
    // Copy constructor
    State(const State& state);
    // Default constructor
    State() {}
    State(string n, bool acc, bool start) : name(n), accepting(acc), starting(start) {}

    State& operator= (const State& copy);

    string name;
    bool accepting = false;
    bool starting = false;
    map<char, vector<State*>> transitions;
};
struct deadState : public State {
    deadState(vector<char>& alphabet);
    bool used = false;
};
class RE;

class DFA {
private:
    json j;
    json dfa1;
    json dfa2;
    bool doorsnede_unie;
    //TFA variable
    json dfa;
    //state elimination
    string reg;
    vector<State*> states;
    vector<State*> acceptingStates;
    State* startState;
    vector<char> alphabet;
    string type;

    DFA* _initCheck;

public:
    bool properlyInitialized();


    //default constructor
    DFA();

    //common functions
    DFA(const string& dfa);
    void print();
    bool accepts(const string& s);
    void addToVector(vector<string>& woorden, string temp);

    //functions for product automaat

    DFA(json v, bool test);
    DFA(const DFA& a, const DFA& b, bool c);
    DFA productAutomaat();
    virtual ~DFA();

    //function tfa
    void printTable();
    DFA minimize();

    //function state elimination
    RE toRE();
    vector<State*> sortStates(const vector<State*>& states);

    //getters
    const string &getReg();

    unsigned long long getStates();

    unsigned long long getAcceptingStates();

    State *getStartState();

    unsigned long long getAlphabet();

    const string &getType();

    DFA* get_initCheck();
    //REQUIRE(this->properlyInitialized(), "DFA wasn't initialized when calling get_initCheck");

    void set_initCheck(DFA* i);
    //REQUIRE(this->properlyInitialized(), "DFA wasn't initialized when calling set_initCheck");

};

#endif //SUBSET_CONSTRUCTION_DFA_H