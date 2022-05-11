//
// Created by aaditya on 11.05.22.
//

#include "ENFA.h"
#include "json.hpp"
#include "fstream"
#include "map"

using namespace std;
using json = nlohmann::json;

ENFA::ENFA(string p) {
    ifstream input(p);
    input >> enfa;
    eps = enfa["eps"];
}


void ENFA::nextNodes(vector<int>* nodes, string input){
    vector<int> new_states;
    for(auto node : *nodes){
        for(auto transition : enfa["transitions"]){
            if(transition["from"] == node && transition["input"] == input && count(new_states.begin(), new_states.end(), transition["to"]) == 0)
                new_states.push_back(transition["to"]);
        }
    }
    *nodes = new_states;
}

void ENFA::tryEps(vector<int>* nodes){
    for(auto node : *nodes){
        for(auto transition : enfa["transitions"]){
            if(transition["from"] == node && transition["input"] == eps && count(nodes->begin(), nodes->end(), transition["to"]) == 0){
                nodes->push_back(transition["to"]);
                return tryEps(nodes);
            }
        }
    }
    return;
}


bool ENFA::accepts(string input){
    vector<int> states = {0};
    tryEps(&states);
    for(auto c : input){
        string character(1, c);
        nextNodes(&states, character);
        tryEps(&states);
    }

    return(count(states.begin(), states.end(), enfa["states"].size()-1)==1);
}

int ENFA::transitionCount(string elem){
    int count = 0;
    for(auto transition : enfa["transitions"]){
        if(transition["input"] == elem)
            count++;
    }
    return count;
}

int ENFA::printDegree(int degree){
    vector<int> vec(enfa["states"].size());
    for(int i = 0; i < enfa["states"].size(); i++){
        for(auto transition : enfa["transitions"]){
            if(enfa["states"][i]["name"] == transition["from"] && transition["from"] != transition["to"])
                vec[i]++;
        }
    }

    return count(vec.begin(), vec.end(), degree);
}
void ENFA::printStats(){
    cout << "no_of_states=" << enfa["states"].size() << endl;

    cout << "no_of_transitions[" << eps << "]=" << transitionCount(eps) << endl;

    vector<string> alph = enfa["alphabet"];
    for(auto elem : alph){
        cout << "no_of_transitions[" << elem << "]=" << transitionCount(elem) << endl;
    }

    int check = 0;
    int i = 0;
    while(check < enfa["states"].size()){
        int result = printDegree(i);
        cout << "degree[" << i << "]=" << result << endl;
        check += result;
        i++;
    }
}

bool ENFA::accept(vector<string> new_state) {
    bool accepting = false;
    for (int i = 0; i < enfa["states"].size(); i++) {
        if (enfa["states"][i]["accepting"] == true) {
            if (count(new_state.begin(), new_state.end(), enfa["states"][i]["name"]))
                accepting = true;
        }
    }
    return accepting;
}

vector<string> ENFA::tryEpsilon(vector<string> state1){
    vector<string> new_state = state1;

    for(auto transition : enfa["transitions"]){
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