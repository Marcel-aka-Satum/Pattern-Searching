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
    bool unieOrDoorsnede;
    //TFA variable
    json dfa;
    //state elimination
    string reg;
    vector<State*> states;
    vector<State*> acceptingStates;
    State* startState;
    vector<char> alphabet;
    string type;

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

    virtual ~DFA();

    //function tfa
    void printTable();
    DFA minimize();

    //function state elimination
    RE toRE(char epsilon);
    vector<State*> sortStates(const vector<State*>& states) const;
    static bool ignore_eps(State* state,const std::pair<const char, vector<State*>> trans, char epsilon);
};

#endif //SUBSET_CONSTRUCTION_DFA_H