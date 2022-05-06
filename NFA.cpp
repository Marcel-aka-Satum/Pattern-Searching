//
// Created by aaditya on 12.04.22.
//

#include "NFA.h"
using json = nlohmann::json;

NFA::NFA(string filename){
    ifstream input(filename);
    input >> k;
    input.close();
}

DFA NFA::toDFA() {
    d = {
            {"type" , "DFA"},
            {"alphabet", k["alphabet"]}
    };
    vector<string> start_state;
    for(int i = 0; i < k["states"].size();i++){
        if(k["states"][i]["starting"] == true){
            start_state = {k["states"][i]["name"]};
        }
    }

    vind_states(start_state);
    for(auto i: new_states){
        if(i == start_state){
            addStates(vecTostring(i),true,accept(i));
        }
        else{
            addStates(vecTostring(i), false, accept(i));
        }
    }
    ofstream output("output-ssc1.json");
    output << d;
    output.close();
    return DFA("output-ssc1.json");
}
void NFA::addStates(string name, bool starting, bool accepting) {
        d["states"].push_back({{"name",name},
                               {"starting",starting},
                               {"accepting",accepting}});
}

void NFA::addTransition(string from, string to, string input) {
    d["transitions"].push_back({{"from",from},
                                {"to",to},
                                {"input",input}});
}

string NFA::vecTostring(vector<string> states) {
    string state = "{" + states[0];
    for(int i = 1; i < states.size(); i++){
        state += "," + states[i];
    }
    state += "}";
    return state;
}

vector<string> NFA::vind_transities(vector<string> state, string input) {
    vector<string> n_state;
    for(int i = 0; i < k["transitions"].size(); i++) {
        if(count(state.begin(), state.end(), k["transitions"][i]["from"]) && k["transitions"][i]["input"]==input){
            n_state.push_back(k["transitions"][i]["to"]);
        }
    }
    sort(n_state.begin(), n_state.end());
    n_state.erase(unique(n_state.begin(),n_state.end()), n_state.end());
    addTransition(vecTostring(state), vecTostring(n_state), input);
    return n_state;
}

void NFA::vind_states(vector<string> const &state) {
    vector<vector<string>> states;

    if(new_states.find(state) != new_states.end()){
        return;
    }

    new_states.insert(state);

    for(auto const &x : d["alphabet"]){
        states.push_back(vind_transities(state, x));
    }

    for(auto const &z : states){
        vind_states(z);
    }
}

bool NFA::accept(vector<string> state) {
    for(int i = 0; i < k["states"].size();i++){
        if(k["states"][i]["accepting"] == true){
            if(count(state.begin(), state.end(), k["states"][i]["name"])){
                return true;
            }
        }
    }
    return false;
}
