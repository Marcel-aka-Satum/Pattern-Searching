#include "ENFA.h"
#include "json.hpp"
#include "fstream"
#include "map"

using namespace std;
using json = nlohmann::json;

ENFA::ENFA(const string& p) {
    ifstream input(p);
    input >> j;
    eps = j["eps"];
}


void ENFA::nextNodes(vector<int>* nodes, string input){
    vector<int> new_states;
    for(auto node : *nodes){
        for(auto transition : j["transitions"]){
            if(transition["from"] == node && transition["input"] == input && count(new_states.begin(), new_states.end(), transition["to"]) == 0)
                new_states.push_back(transition["to"]);
        }
    }
    *nodes = new_states;
}

void ENFA::tryEps(vector<int>* nodes){
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
    vector<int> states = {0};
    tryEps(&states);
    for(auto c : input){
        string character(1, c);
        nextNodes(&states, character);
        tryEps(&states);
    }

    return(count(states.begin(), states.end(), j["states"].size() - 1) == 1);
}

int ENFA::transitionCount(string elem){
    int count = 0;
    for(auto transition : j["transitions"]){
        if(transition["input"] == elem)
            count++;
    }
    return count;
}

int ENFA::printDegree(int degree){
    vector<int> vec(j["states"].size());
    for(int i = 0; i < j["states"].size(); i++){
        for(auto transition : j["transitions"]){
            if(j["states"][i]["name"] == transition["from"] && transition["from"] != transition["to"])
                vec[i]++;
        }
    }

    return count(vec.begin(), vec.end(), degree);
}
void ENFA::printStats(){
    cout << "no_of_states=" << j["states"].size() << endl;

    cout << "no_of_transitions[" << eps << "]=" << transitionCount(eps) << endl;

    vector<string> alph = j["alphabet"];
    for(auto elem : alph){
        cout << "no_of_transitions[" << elem << "]=" << transitionCount(elem) << endl;
    }

    int check = 0;
    int i = 0;
    while(check < j["states"].size()){
        int result = printDegree(i);
        cout << "degree[" << i << "]=" << result << endl;
        check += result;
        i++;
    }
}

bool ENFA::accept(vector<string> new_state) {
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
    cout << setw(4) << j << endl;
}

DFA ENFA::toDFA() {
    json l;
    l["type"] = "DFA";
    l["alphabet"] = j["alphabet"];
    string currentState = "{";
    vector<json> states;
    vector<json> transitions;
    string startingState;
    string acceptingStates;
    bool startingAcceptBool;

    // hier kijk ik wat mijn starting state is en zet ik dat state true en geef dat door aan de var currentState
    for(int i = 0; i < j["states"].size(); i++){
        if (j["states"][i]["starting"] == true){
            currentState = j["states"][i]["name"];
            currentState += "}";
            startingState = j["states"][i]["name"];
            startingAcceptBool = j["states"][i]["accepting"];
        }
        if(j["states"][i]["accepting"] == true){
            acceptingStates += j["states"][i]["name"];
        }
    }
    queue<string> epsilon;
    epsilon.push(startingState);
    while (!epsilon.empty()){
        string test = epsilon.front();
        epsilon.pop();
        for (int k = 0; k < j["transitions"].size(); k++) {
            if ((j["transitions"][k]["from"] == test) and (j["transitions"][k]["input"] == "*")) {
                bool found = false;
                for (int m = 0; m < currentState.size(); ++m) {
                    string ok(1,currentState[m]);
                    if (ok == j["transitions"][k]["to"]){
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    currentState += j["transitions"][k]["to"];
                    startingState += ",";
                    startingState += j["transitions"][k]["to"];
                    currentState += ",";
                    epsilon.push(j["transitions"][k]["to"]);
                }

            }
        }
    }
    queue<string> notDoneStates;
    notDoneStates.push(currentState);
    vector<string> doneStates;
    json r;
    string tempS = "{" + startingState + "}";;
    r["name"] = tempS;
    r["starting"] = true;
    r["accepting"] = startingAcceptBool;
    states.push_back(r);
    while (!notDoneStates.empty()){
        currentState = notDoneStates.front();
        notDoneStates.pop();
        vector<string> newStates;
        bool starting = false;
        bool accepting = false;
        for (int i = 0; i < j["alphabet"].size(); ++i) {
            string inp = j["alphabet"][i];
            string newState = "{";
            for (int i = 0; i < currentState.size(); ++i) {
                if (currentState[i] != '{' and currentState[i] != '}') {
                    for (int k = 0; k < j["transitions"].size(); k++) {
                        string state(1, currentState[i]);
                        if ((j["transitions"][k]["from"] == state) and (j["transitions"][k]["input"] == inp)) {
                            newState += j["transitions"][k]["to"];
                            newState += ",";
                        }
                    }
                }
            }
            unordered_set<char> log;
            newState.erase(remove_if(newState.begin(), newState.end(), [&] (char const c) { return !(log.insert(c).second); }), newState.end());
            queue<string> epsilon;

            for (int k = 0; k < newState.size(); ++k) {
                string v(1,newState[k]);
                if (v != "{")
                    epsilon.push(v);
            }

            while (!epsilon.empty()){
                string test = epsilon.front();
                epsilon.pop();
                for (int k = 0; k < j["transitions"].size(); k++) {
                    if ((j["transitions"][k]["from"] == test) and (j["transitions"][k]["input"] == "*")) {
                        bool found = false;
                        for (int m = 0; m < newState.size(); ++m) {
                            string ok(1,newState[m]);
                            if (ok == j["transitions"][k]["to"]){
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            newState += j["transitions"][k]["to"];
                            newState += ",";
                            epsilon.push(j["transitions"][k]["to"]);
                        }

                    }
                }
            }

            newState[newState.size() - 1] = '}';
            newStates.push_back(newState);
        }
        string temp3;
        for (int i = 0; i < currentState.size(); i++){
            if (isdigit(currentState[i])){
                temp3 += currentState[i];
            }
        }
        sort(temp3.begin() , temp3.end());
        currentState = "{";
        for (int j = 0; j < temp3.size(); j++){
            currentState += temp3[j];
            currentState += ",";
        }
        currentState[currentState.size()-1] = '}';

        for (int k = 0; k < newStates.size(); ++k) {
            string temp4;
            for (int i = 0; i < newStates[k].size(); i++) {
                if (isdigit(newStates[k][i])) {
                    temp4 += newStates[k][i];
                }
            }
            sort(temp4.begin(), temp4.end());
            newStates[k] = "{";
            for (int j = 0; j < temp4.size(); j++) {
                newStates[k] += temp4[j];
                newStates[k] += ",";
            }
            newStates[k][newStates[k].size() - 1] = '}';
        }
        if (currentState[0] != '{'){
            string newString = "{";
            for (int i = 0; i < currentState.size() ; ++i) {
                newString+= currentState[i];
            }
            currentState = newString;
        }

        for (int k = 0; k < newStates.size() ; ++k) {
            accepting = false;
            starting = false;
            bool alreadyDone = false;
            for (int i = 0; i < doneStates.size(); ++i) {
                if (newStates[k] == doneStates[i]){
                    alreadyDone = true;
                    break;
                }
            }
            if (!alreadyDone) {
                json temp;
                json temp2;
                temp["name"] = newStates[k];
                if (newStates[k].size() == 3) {
                    string test(1, newStates[k][1]);
                    if (test == startingState) {
                        starting = true;
                    }
                }
                for (int i = 0; i < newStates[k].size(); ++i) {
                    for (int x = 0; x < acceptingStates.size(); ++x) {
                        if (newStates[k][i] == acceptingStates[x]) {
                            accepting = true;
                        }
                    }
                }
                temp["starting"] = starting;
                temp["accepting"] = accepting;
                if (newStates[k] != "}")
                    states.push_back(temp);
                temp2["from"] = currentState;
                temp2["to"] = newStates[k];
                temp2["input"] = j["alphabet"][k];
                if (newStates[k] != "}")
                    transitions.push_back(temp2);
                if (newStates[k] != "}")
                    notDoneStates.push(newStates[k]);
            }
            else {
                json temp;
                temp["from"] = currentState;
                temp["to"] = newStates[k];
                temp["input"] = j["alphabet"][k];
                if (newStates[k] != "}")
                    transitions.push_back(temp);
            }
        }
        doneStates.push_back(currentState);
    }
    sort( states.begin(), states.end() );
    states.erase( unique( states.begin(), states.end() ), states.end() );

    sort( transitions.begin(), transitions.end() );
    transitions.erase( unique( transitions.begin(), transitions.end() ), transitions.end() );
    l["states"] = states;
    l["transitions"] = transitions;
    ofstream output("output-ssc1.json");
    output << l;
    output.close();
    return DFA("output-ssc1.json");
}