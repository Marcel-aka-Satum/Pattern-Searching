#ifndef PATTERN_SEARCHING_RE_H
#define PATTERN_SEARCHING_RE_H
#include "iostream"
#include "ENFA.h"
#include "string"
#include "contract.h"
#include "json.hpp"
#include "fstream"
#include "set"
using namespace std;
using json = nlohmann::json;

class RE {
protected:
    RE *__initCheck;
    string regex;
    string eps;
    json renfa;
    vector<int> states = {0};
    set<string> alph;
public:
    struct block {
        int start;
        int end;
        string expression;
    };

    RE();

    const string &getRegex() const;

    const string &getEps() const;

    const json &getRenfa() const;

    int getStates() const;

    int getAlph() const;

    vector<block> transitions;

    RE(string r, char e);

    ENFA toENFA();

    void print();
    void addTransition(int from, int to, string input);
    void sum(block* R, block* S);
    void concat(block* R, block* S);
    void kleene(block* R);
    void findTransitions(block &a);
    pair<block, block> breakUp(block a);
    static string removeBracket(string expression);
    static bool noBrackets(string const &expression);
    static int calculateOperations(string const &expression);
    void doOperation(block* R, block* S, string const &operation);
    static void findStartEnd(block* a, int* startPoint, int* endpoint);
    static void findExpressions(int startPoint, int endPoint, string* expression1, string* expression2, string* operation, block a);
    static void buildBlock(block* x, block* y,block const &a,string const &expression1,string const &expression2, string const &operation);
    void buildTransitions();
    void buildStates(int end);
    void addState(int name, bool accepting, bool starting);
    void buildAlph();
    static bool noOperators(string const &expression);
    bool properlyInitialized() const;

    RE* get_initCheck();
    //REQUIRE(this->properlyInitialized(), "RE wasn't initialized when calling get_initCheck");

    void set_initCheck(RE* i);
    //REQUIRE(this->properlyInitialized(), "RE wasn't initialized when calling set_initCheck");

};


#endif //PATTERN_SEARCHING_RE_H
