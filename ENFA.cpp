#include "ENFA.h"
#include "json.hpp"
#include "DFA.h"
#include "fstream"
#include "map"

using namespace std;
using json = nlohmann::json;

ENFA::ENFA(const string& p) {
    __initCheck = this;
    ifstream input(p);
    input >> j;
    eps = j["eps"];
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}


void ENFA::nextNodes(vector<string>* nodes, string input){
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling nextNodes");
    vector<string> new_states;
    for(auto node : *nodes){
        for(auto transition : j["transitions"]){
            if(transition["from"] == node && transition["input"] == input && count(new_states.begin(), new_states.end(), transition["to"]) == 0)
                new_states.push_back(transition["to"]);
        }
    }
    *nodes = new_states;
}

void ENFA::tryEps(vector<string>* nodes){
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling tryEps");
    for(auto node : *nodes){
        for(auto transition : j["transitions"]){
            if(transition["from"] == node && transition["input"] == eps && count(nodes->begin(), nodes->end(), transition["to"]) == 0){
                nodes->push_back(transition["to"]);
                return tryEps(nodes);
            }
        }
    }
    return;
}


bool ENFA::accepts(string input){
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling accepts");
    vector<string> states = {to_string(0)};
    tryEps(&states);
    for(auto c : input){
        string character(1, c);
        nextNodes(&states, character);
        tryEps(&states);
    }
    int nrtimes = 0;
    for(int i = 0; i < states.size(); i++){
        if(to_string(j["states"].size()-1) == states[i]){
            nrtimes++;
        }
    }
    return(nrtimes==1);
}
bool ENFA::accept(vector<string> new_state) {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling accept");
    bool accepting = false;
    for (int i = 0; i < j["states"].size(); i++) {
        if (j["states"][i]["accepting"] == true) {
            if (count(new_state.begin(), new_state.end(), j["states"][i]["name"]))
                accepting = true;
        }
    }
    return accepting;
}

vector<string> ENFA::tryEpsilon(vector<string> state1){
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling tryEpsilon");
    vector<string> new_state = state1;

    for(auto transition : j["transitions"]){
        if(count(state1.begin(), state1.end(), transition["from"]) && transition["input"] == eps)
            new_state.push_back(transition["to"]);
    }
    sort(new_state.begin(), new_state.end());
    new_state.erase( unique( new_state.begin(), new_state.end() ), new_state.end() );

    if(new_state == state1)
        return new_state;
    else
        return tryEpsilon(new_state);
}

//functions for mssc

bool ENFA::accept(const string &s) {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling accept");
    bool states[j["states"].size()];
    string currentState;
    // hier kijk ik wat mijn starting state is en zet ik dat state true en geef dat door aan de var currentState
    for(int i = 0; i < j["states"].size(); i++){
        if (j["states"][i]["starting"] == true){
            states[i] = true;
            currentState = j["states"][i]["name"];
            break;
        }
    }
    // hier kijk ik of de char in de alphabet zit, zoniet dn zet ik de curr state nr een dead state.
    for (auto i: s) {
        string inp(1,i);
        bool inAlphabet = false;
        for (const auto & l : j["alphabet"]){
            if (inp == l){
                inAlphabet = true;
            }
        }
        if (!inAlphabet){
            currentState = "DEAD STATE";
        }
        // hier kijk ik welke transitie het moet doen, door te kijken wat de currentState is en de input.
        for (int k = 0; k < j["transitions"].size(); k++){
            if ((j["transitions"][k]["from"] == currentState) and (j["transitions"][k]["input"] == inp)){
                currentState = j["transitions"][k]["to"];
                break;
            }
        }
    }
    // hier check ik waar mijn currentState is geindigd en of dat state een accepting state is, zoja return ik true anders false.
    for(int i = 0; i < j["states"].size(); i++){
        if (j["states"][i]["name"] == currentState){
            if (j["states"][i]["accepting"] == true)
                return true;
            else
                return false;
        }
    }
    return false;

}

void ENFA::print() {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling print");
    cout << setw(4) << j << endl;
}

DFA ENFA::toDFA() {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling toDFA");
    vector<string> startState;
    dfa = {
            {"type",     "DFA"},
            {"alphabet", j["alphabet"]}
    };

    for (int i = 0; i < j["states"].size(); i++) {
        if (j["states"][i]["starting"] == true) {
            startState = {j["states"][i]["name"]};
            startState = tryEpsilon(startState);
            dfa["states"] = {"", ""};
        }
    }
    dfa["transitions"] = {"", ""};
    subsetConstruction(startState);

    for (auto const &elem : allStates) {
        if (elem == startState)
            addState(vecToString(elem), true, accept(elem));
        else
            addState(vecToString(elem), false, accept(elem));
    }

    //verwijder de blank spaces
    dfa["states"].erase(dfa["states"].begin());
    dfa["states"].erase(dfa["states"].begin());
    dfa["transitions"].erase(dfa["transitions"].begin());
    dfa["transitions"].erase(dfa["transitions"].begin());

    ofstream file("dfa-output.json");
    file << dfa;
    file.close();
    DFA dfaa("dfa-output.json");
    return dfaa;
}
void ENFA::subsetConstruction(vector<string> const &state) {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling subsetConstruction");
    vector<vector<string>> states;

    if (allStates.find(state) != allStates.end())
        return;
    allStates.insert(state);

    for (auto const &alph : dfa["alphabet"]) {
        vector<string> new_state = tryEpsilon(findTransition(state, alph));
        states.push_back(new_state);
        addTransition(vecToString(state), vecToString(new_state), alph);
    }

    for (auto const &s : states) {
        subsetConstruction(s);
    }
}
string ENFA::vecToString(vector<string> new_state) {
    vector<int> temp;
    vector<string> tempr;
    if (new_state.empty())
        return "{}";
    if(std::isdigit(new_state[0][0])) {
        for (int j = 0; j < new_state.size(); j++) {
            temp.push_back(stoi(new_state[j]));
        }
        sort(temp.begin(), temp.end());
        for (int k = 0; k < temp.size(); k++) {
            tempr.push_back(to_string(temp[k]));
        }
        string name = "{" + tempr[0];
        for (int i = 1; i < tempr.size(); i++) {
            name += ",";
            name += tempr[i];
        }
        name += "}";
        return name;
    }
    else{
        sort(new_state.begin(),new_state.end());
        string name = "{" + new_state[0];
        for (int i = 1; i < new_state.size(); i++) {
            name += ",";
            name += new_state[i];
        }
        name += "}";
        return name;
    }
}
vector<string> ENFA::findTransition(vector<string> state, string input) {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling findTransition");
    vector<string> new_state;
    for (int i = 0; i < j["transitions"].size(); i++) {
        if (count(state.begin(), state.end(), j["transitions"][i]["from"]) &&
            j["transitions"][i]["input"] == input) {
            new_state.push_back(j["transitions"][i]["to"]);
        }
    }
    return new_state;
}
void ENFA::addTransition(string from, string to, string input) {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling addTransition");
    dfa["transitions"].push_back(
            {{"from",  from},
             {"to",    to},
             {"input", input}});
}
void ENFA::addState(string name, bool starting, bool accepting) {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling addState");
    dfa["states"].push_back(
            {{"name",      name},
             {"starting",  starting},
             {"accepting", accepting}});
}

const string &ENFA::getEps() const {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling getEps");
    return eps;
}

int ENFA::getAllStates() const {
    REQUIRE(this->properlyInitialized(), "Wasn't initialized when calling getAllStates");
    return allStates.size();
}

ENFA::ENFA() {
    __initCheck = this;
    ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

bool ENFA::properlyInitialized() const {
    return __initCheck == this;
}

ENFA *ENFA::get_initCheck() {
    REQUIRE(this->properlyInitialized(), "ENFA wasn't initialized when calling get_initCheck");
    return __initCheck;
}

void ENFA::set_initCheck(ENFA* i) {
    REQUIRE(this->properlyInitialized(), "ENFA wasn't initialized when calling set_initCheck");
    __initCheck = i;
}