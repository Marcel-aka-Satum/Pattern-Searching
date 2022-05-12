#include "DFA.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;

// common functions
DFA::DFA(const string& dfa){
    ifstream input(dfa);
    input >> j;
}
void DFA::print(){
    cout << setw(4) << j << endl;
}
bool DFA::accepts(const string &s) {
    bool states[j["states"].size()];
    string currentState;
    // hier kijk ik wat mijn starting state is en zet ik dat state true en geef dat door aan de var currentState
    for(int i = 0; i < j["states"].size(); i++){
        if (j["states"][i]["starting"] == true){
            states[i] = true;
            currentState = j["states"][i]["name"];
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
// functions for product automaat
DFA::DFA(json v, bool test) {
    j = std::move(v);
}

DFA::DFA(const DFA &a, const DFA &b, bool c) {
    dfa1 = a.j;
    dfa2 = b.j;
    unieOrDoorsnede = c;
    if (c)
        doDoorsnede();
    else
        doUnie();
}

void DFA::doUnie() {
    vector<string> allStates;
    vector<json> allTransitions;
    for (int i = 0; i < dfa1["states"].size(); ++i) {
        for (int k = 0; k < dfa2["states"].size(); ++k) {
            string newState = "(";
            newState += dfa1["states"][i]["name"];
            newState += ",";
            newState += dfa2["states"][k]["name"];
            newState += ")";
            allStates.push_back(newState);
        }
    }
    for (int i = 0; i < dfa1["alphabet"].size(); ++i) {
        for (int k = 0; k < allStates.size(); ++k) {
            json t;
            string s1 (1,allStates[k][1]);
            string s2 (1, allStates[k][3]);
            string toState = "(";
            for (int l = 0; l < dfa1["transitions"].size(); ++l) {
                if (dfa1["transitions"][l]["from"] == s1 and dfa1["transitions"][l]["input"] == dfa1["alphabet"][i]){
                    toState += dfa1["transitions"][l]["to"];
                    toState += ",";
                }
            }
            for (int l = 0; l < dfa2["transitions"].size(); ++l) {
                if (dfa2["transitions"][l]["from"] == s2 and dfa2["transitions"][l]["input"] == dfa1["alphabet"][i]){
                    toState += dfa2["transitions"][l]["to"];
                    toState += ")";
                }
            }
            t["from"] = allStates[k];
            t["to"] = toState;
            t["input"] = dfa1["alphabet"][i];
            allTransitions.push_back(t);
        }
    }
    vector<string> unreachableStates;
    string startingState = "(";
    for (int i = 0; i < dfa1["states"].size(); ++i) {
        if (dfa1["states"][i]["starting"] == true){
            startingState += dfa1["states"][i]["name"];
            startingState += ",";
        }
    }
    for (int i = 0; i < dfa2["states"].size(); ++i) {
        if (dfa2["states"][i]["starting"] == true){
            startingState += dfa2["states"][i]["name"];
            startingState += ")";
        }
    }
    int count = dfa1["states"].size() * dfa2["states"].size() * dfa1["alphabet"].size();
    for (int l = 0; l < count; ++l) {
        for (int i = 0; i < allStates.size(); ++i) {
            if (allStates[i] != startingState) {
                bool isUnreachable = true;
                for (int k = 0; k < allTransitions.size(); ++k) {
                    if (allTransitions[k]["to"] == allStates[i]) {
                        isUnreachable = false;
                    }
                }
                if (isUnreachable) {
                    unreachableStates.push_back(allStates[i]);
                    allStates.erase(allStates.begin() + i);
                }
            }
        }
        for (int i = 0; i < unreachableStates.size(); ++i) {
            for (int k = 0; k < allTransitions.size(); ++k) {
                if (unreachableStates[i] == allTransitions[k]["from"]){
                    allTransitions.erase(allTransitions.begin() + k);
                }
            }
        }
    }
    for (int i = 0; i < allStates.size(); ++i) {
        string temp;
        bool selfLoop = false;
        for (int k = 0; k < allTransitions.size(); ++k) {
            if (allTransitions[k]["from"] == allStates[i]){
                temp = allTransitions[k]["to"];
            }
        }
        for (int k = 0; k < allTransitions.size(); ++k) {
            if (allTransitions[k]["from"] == temp){
                if (allTransitions[k]["to"] == allStates[i]){
                    selfLoop = true;
                }
            }
        }
        if (selfLoop){
            bool isUnreachable = true;
            for (int k = 0; k < allTransitions.size(); ++k) {
                if (allTransitions[k]["to"] == allStates[i] and allTransitions[k]["from"] != temp){
                    isUnreachable = false;
                }
            }
            if (isUnreachable){
                unreachableStates.push_back(allStates[i]);
                allStates.erase(allStates.begin() + i);
                for (int k = 0; k < allStates.size(); ++k) {
                    if (allStates[k] == temp){
                        unreachableStates.push_back(allStates[k]);
                        allStates.erase(allStates.begin() + k);
                        break;
                    }
                }
            }
            break;
        }
    }
    for (int i = 0; i < unreachableStates.size(); ++i) {
        for (int k = 0; k < allTransitions.size(); ++k) {
            if (unreachableStates[i] == allTransitions[k]["from"]){
                allTransitions.erase(allTransitions.begin() + k);
            }
        }
    }
    for (int l = 0; l < count; ++l) {
        for (int i = 0; i < allStates.size(); ++i) {
            if (allStates[i] != startingState) {
                bool isUnreachable = true;
                for (int k = 0; k < allTransitions.size(); ++k) {
                    if (allTransitions[k]["to"] == allStates[i]) {
                        isUnreachable = false;
                    }
                }
                if (isUnreachable) {
                    unreachableStates.push_back(allStates[i]);
                    allStates.erase(allStates.begin() + i);
                }
            }
        }
        for (int i = 0; i < unreachableStates.size(); ++i) {
            for (int k = 0; k < allTransitions.size(); ++k) {
                if (unreachableStates[i] == allTransitions[k]["from"]){
                    allTransitions.erase(allTransitions.begin() + k);
                }
            }
        }
    }
    vector<json> states;
    for (int i = 0; i < allStates.size(); ++i) {
        string s1(1,allStates[i][1]);
        string s2(1, allStates[i][3]);
        json s;
        bool starting = false;
        bool accepting = false;
        if (allStates[i] == startingState){
            starting = true;
        }
        for (int k = 0; k < dfa1["states"].size(); ++k) {
            if (dfa1["states"][k]["name"] == s1){
                accepting = dfa1["states"][k]["accepting"];
                break;
            }
        }
        if (!accepting) {
            for (int k = 0; k < dfa2["states"].size(); ++k) {
                if (dfa2["states"][k]["name"] == s2) {
                    accepting = dfa2["states"][k]["accepting"];
                    break;
                }
            }
        }
        s["name"] = allStates[i];
        s["starting"] = starting;
        s["accepting"] = accepting;
        states.push_back(s);
    }
    j["type"] = "DFA";
    j["alphabet"] = dfa1["alphabet"];
    j["states"] = states;
    j["transitions"] = allTransitions;
}

void DFA::doDoorsnede() {
    vector<string> allStates;
    vector<json> allTransitions;
    for (int i = 0; i < dfa1["states"].size(); ++i) {
        for (int k = 0; k < dfa2["states"].size(); ++k) {
            string newState = "(";
            newState += dfa1["states"][i]["name"];
            newState += ",";
            newState += dfa2["states"][k]["name"];
            newState += ")";
            allStates.push_back(newState);
        }
    }
    for (int i = 0; i < dfa1["alphabet"].size(); ++i) {
        for (int k = 0; k < allStates.size(); ++k) {
            json t;
            string s1 (1,allStates[k][1]);
            string s2 (1, allStates[k][3]);
            string toState = "(";
            for (int l = 0; l < dfa1["transitions"].size(); ++l) {
                if (dfa1["transitions"][l]["from"] == s1 and dfa1["transitions"][l]["input"] == dfa1["alphabet"][i]){
                    toState += dfa1["transitions"][l]["to"];
                    toState += ",";
                }
            }
            for (int l = 0; l < dfa2["transitions"].size(); ++l) {
                if (dfa2["transitions"][l]["from"] == s2 and dfa2["transitions"][l]["input"] == dfa1["alphabet"][i]){
                    toState += dfa2["transitions"][l]["to"];
                    toState += ")";
                }
            }
            t["from"] = allStates[k];
            t["to"] = toState;
            t["input"] = dfa1["alphabet"][i];
            allTransitions.push_back(t);
        }
    }
    vector<string> unreachableStates;
    string startingState = "(";
    for (int i = 0; i < dfa1["states"].size(); ++i) {
        if (dfa1["states"][i]["starting"] == true){
            startingState += dfa1["states"][i]["name"];
            startingState += ",";
        }
    }
    for (int i = 0; i < dfa2["states"].size(); ++i) {
        if (dfa2["states"][i]["starting"] == true){
            startingState += dfa2["states"][i]["name"];
            startingState += ")";
        }
    }
    int count = dfa1["states"].size() * dfa2["states"].size() * dfa1["alphabet"].size();
    for (int l = 0; l < count; ++l) {
        for (int i = 0; i < allStates.size(); ++i) {
            if (allStates[i] != startingState) {
                bool isUnreachable = true;
                for (int k = 0; k < allTransitions.size(); ++k) {
                    if (allTransitions[k]["to"] == allStates[i]) {
                        isUnreachable = false;
                    }
                }
                if (isUnreachable) {
                    unreachableStates.push_back(allStates[i]);
                    allStates.erase(allStates.begin() + i);
                }
            }
        }
        for (int i = 0; i < unreachableStates.size(); ++i) {
            for (int k = 0; k < allTransitions.size(); ++k) {
                if (unreachableStates[i] == allTransitions[k]["from"]){
                    allTransitions.erase(allTransitions.begin() + k);
                }
            }
        }
    }
    for (int i = 0; i < allStates.size(); ++i) {
        string temp;
        bool selfLoop = false;
        for (int k = 0; k < allTransitions.size(); ++k) {
            if (allTransitions[k]["from"] == allStates[i]){
                temp = allTransitions[k]["to"];
            }
        }
        for (int k = 0; k < allTransitions.size(); ++k) {
            if (allTransitions[k]["from"] == temp){
                if (allTransitions[k]["to"] == allStates[i]){
                    selfLoop = true;
                }
            }
        }
        if (selfLoop){
            bool isUnreachable = true;
            for (int k = 0; k < allTransitions.size(); ++k) {
                if (allTransitions[k]["to"] == allStates[i] and allTransitions[k]["from"] != temp){
                    isUnreachable = false;
                }
            }
            if (isUnreachable){
                unreachableStates.push_back(allStates[i]);
                allStates.erase(allStates.begin() + i);
                for (int k = 0; k < allStates.size(); ++k) {
                    if (allStates[k] == temp){
                        unreachableStates.push_back(allStates[k]);
                        allStates.erase(allStates.begin() + k);
                        break;
                    }
                }
            }
            break;
        }
    }
    for (int i = 0; i < unreachableStates.size(); ++i) {
        for (int k = 0; k < allTransitions.size(); ++k) {
            if (unreachableStates[i] == allTransitions[k]["from"]){
                allTransitions.erase(allTransitions.begin() + k);
            }
        }
    }
    for (int l = 0; l < count; ++l) {
        for (int i = 0; i < allStates.size(); ++i) {
            if (allStates[i] != startingState) {
                bool isUnreachable = true;
                for (int k = 0; k < allTransitions.size(); ++k) {
                    if (allTransitions[k]["to"] == allStates[i]) {
                        isUnreachable = false;
                    }
                }
                if (isUnreachable) {
                    unreachableStates.push_back(allStates[i]);
                    allStates.erase(allStates.begin() + i);
                }
            }
        }
        for (int i = 0; i < unreachableStates.size(); ++i) {
            for (int k = 0; k < allTransitions.size(); ++k) {
                if (unreachableStates[i] == allTransitions[k]["from"]){
                    allTransitions.erase(allTransitions.begin() + k);
                }
            }
        }
    }
    vector<json> states;
    for (int i = 0; i < allStates.size(); ++i) {
        string s1(1,allStates[i][1]);
        string s2(1, allStates[i][3]);
        json s;
        bool starting = false;
        bool accepting = false;
        if (allStates[i] == startingState){
            starting = true;
        }
        for (int k = 0; k < dfa1["states"].size(); ++k) {
            if (dfa1["states"][k]["name"] == s1){
                accepting = dfa1["states"][k]["accepting"];
                break;
            }
        }
        if (accepting) {
            for (int k = 0; k < dfa2["states"].size(); ++k) {
                if (dfa2["states"][k]["name"] == s2) {
                    accepting = dfa2["states"][k]["accepting"];
                    break;
                }
            }
        }
        s["name"] = allStates[i];
        s["starting"] = starting;
        s["accepting"] = accepting;
        states.push_back(s);
    }
    j["type"] = "DFA";
    j["alphabet"] = dfa1["alphabet"];
    j["states"] = states;
    j["transitions"] = allTransitions;
}

//functions for mssc

/*DFA::DFA(json v) {
    j = std::move(v);
}*/
