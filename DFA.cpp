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

//function tfa

DFA DFA::minimize() {
    vector <json> states;
    vector <json> transitions;


    vector<string> del;


    for (auto i: j["states"]) {
        names2.push_back(i["name"]);
    }
    sort(names2.begin(), names2.end());


    //Accepting check
    for (int i = 1; i < j["states"].size(); i++) {
        bool acc1;
        bool acc2;
        for (auto k: j["states"]) {
            if (k["name"] == names2[i]) {
                acc1 = k["accepting"];
            }
        }

        int temp = 0;
        bool stop = false;
        while (!stop) {
            for (auto l: j["states"]) {
                if (l["name"] == names2[temp]) {
                    acc2 = l["accepting"];
                }
            }

            if (acc1 != acc2) {
                string comb = "";
                comb += names2[i];
                comb += names2[temp];
                sort(comb.begin(), comb.end());
                checkerMini.push_back(comb);
            }

            if (temp == i) {
                stop = true;
            } else {
                temp += 1;
            }
        }
    }


    string tup;
    bool end = false;

    //Transitie check
    while (!end) {
        int counter = 0;
        for (auto i: j["alphabet"]) {
            for (auto k: names2) {
                for (auto l: names2) {
                    tup = "";
                    tup += k;
                    tup += l;
                    sort(tup.begin(), tup.end());

                    string temp2 = "";


                    if (k != l) {
                        for (auto o: j["transitions"]) {
                            if (k == o["from"] && i == o["input"]) {
                                temp2 += o["to"];
                            }
                            if (l == o["from"] && i == o["input"]) {
                                temp2 += o["to"];
                            }
                        }
                        sort(temp2.begin(), temp2.end());

                        int erin = 0;
                        for (auto p: checkerMini) {
                            if (tup == p) {
                                erin = 1;
                            }
                        }
                        if (erin == 0) {
                            for (auto p: checkerMini) {
                                if (temp2 == p) {
                                    checkerMini.push_back(tup);
                                    counter += 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (counter == 0) {
            end = true;
        }
    }




    for (auto i: names2) {
        for (auto j: names2) {
            int ct = 0;
            if (i != j) {
                string pot = "";
                pot += i;
                pot += j;
                sort(pot.begin(), pot.end());
                for (auto l: checkerMini) {
                    if (l == pot) {
                        ct += 1;
                    }
                }
                for (auto k: over) {
                    if (k == pot) {
                        ct += 1;
                    }
                }
                if (ct == 0) {
                    over.push_back(pot);
                }
            }

        }
    }

    /*for(auto i : over){
        cout << "i: " << i << endl;
    }*/

    //single states toevoegen aan over vector
    string str;
    for (auto i: over) {
        str += i;
    }

    for (auto i: names2) {
        int counter3 = 0;
        for (auto k: str) {
            string str2 = "";
            str2 += k;
            if (i == str2) {
                counter3 += 1;
            }
        }
        if (counter3 == 0) {
            over.push_back(i);
        }
    }



    string str1 = "";
    string tempor = "";
    for (auto i: over) {
        string str1 = i;
        for (auto l: over) {
            string str2 = l;
            for (auto k: str1) {
                for (auto p: str2) {
                    if (k == p && str1 != str2) {
                        tempor += str1;
                        tempor += str2;
                    }
                }
            }
        }
    }
    for (int k = 0; k < tempor.size(); k++) {
        for (int l = k + 1; l < tempor.size(); l++) {
            if (tempor[k] == tempor[l]) {
                char temp = tempor[k];
                tempor.erase(remove(tempor.begin(), tempor.end(), tempor[k]), tempor.end());
                tempor += temp;
            }
        }
    }
    sort(tempor.begin(), tempor.end());
    //cout << "tempor: " << tempor << endl;
    for(auto i : over){
        int ctrr = 0;
        for(auto l : i){
            for(auto k : tempor){
                //cout << "l: " << l << "k: " << k << endl;
                if(l == k){
                    ctrr += 1;
                }
            }
        }
        if(ctrr == i.size()){
            del.push_back(i);
        }
    }
    for(auto i : del){
        over.erase(remove(over.begin(), over.end(), i), over.end());
    }
    over.push_back(tempor);




    /*for(auto i : over){
        cout << "z: " << i << endl;
    }*/

    //dfa states
    bool accept;
    bool start;
    for(auto i : over){
        for(auto k : j["states"]){
            json state;
            string ter = "";
            ter += i[0];

            if(i.size() == 1 and k["name"] == i){
                accept = k["accepting"];
                start = k["starting"];

                string name = "{";
                name += i + "}";

                state["name"] = name;
                state["accepting"] = accept;
                state["starting"] = start;
                states.push_back(state);
                break;
            }else if(i.size() > 1 and k["name"] == ter){
                accept = k["accepting"];
                start = k["starting"];

                int countertje2 = 0;
                string name2 = "{";
                for(auto m : i){
                    name2 += m;
                    countertje2 += 1;
                    if(countertje2 != i.size()){
                        name2 += ", ";
                    }
                }
                name2 += "}";

                state["name"] = name2;
                state["accepting"] = accept;
                state["starting"] = start;
                states.push_back(state);
                break;
            }

        }
    }


    //dfa transitions
    for(auto i : over){
        for(auto l : j["alphabet"]) {
            for (auto k: j["transitions"]) {
                string ter = "";
                ter += i[0];

                //single states
                if (i.size() == 1 and k["from"] == i and k["input"] == l) {
                    json trans;
                    string name3 = "{";
                    name3 += i + "}";
                    trans["from"] = name3;

                    string po = "";
                    po += k["to"];

                    for(auto u : over){
                        for(auto p : u){
                            string yu = "";
                            yu += p;
                            if(yu == po){
                                po = u;
                            }
                        }
                    }

                    int countertje4 = 0;
                    string name4 = "{";
                    for(auto m : po){
                        name4 += m;
                        countertje4 += 1;
                        if(countertje4 != po.size()){
                            name4 += ", ";
                        }
                    }
                    name4 += "}";


                    trans["to"] = name4;
                    trans["input"] = l;
                    transitions.push_back(trans);
                }//combined states
                else if(i.size() > 1 and k["from"] == ter and k["input"] == l){
                    json trans;
                    int countertje5 = 0;
                    string name5 = "{";
                    for(auto m : i){
                        name5 += m;
                        countertje5 += 1;
                        if(countertje5 != i.size()){
                            name5 += ", ";
                        }
                    }
                    name5 += "}";

                    trans["from"] = name5;
                    string top;
                    top += k["to"];

                    string qui;
                    for(auto h : over){
                        for(auto m : h){
                            string tp = "";
                            tp += m;
                            if(top == tp){
                                qui = "";
                                qui = h;
                            }
                        }
                    }

                    int countertje6 = 0;
                    string name6 = "{";
                    for(auto m : qui){
                        name6 += m;
                        countertje6 += 1;
                        if(countertje6 != qui.size()){
                            name6 += ", ";
                        }
                    }
                    name6 += "}";

                    trans["to"] = name6;
                    trans["input"] = l;
                    transitions.push_back(trans);

                }
            }
        }
    }

    dfa["type"] = "DFA";
    dfa["alphabet"] = j["alphabet"];
    dfa["states"] = states;
    dfa["transitions"] = transitions;


    ofstream output("tfa.json");
    output << dfa;
    output.close();

    return DFA("tfa.json");
}
void DFA::printTable() {
    for(auto i : j["states"]){
        names.push_back(i["name"]);
    }
    sort(names.begin(), names.end());


    //Accepting check
    for(int i = 1; i < j["states"].size(); i++){
        bool acc1;
        bool acc2;
        for(auto k : j["states"]){
            if(k["name"] == names[i]){
                acc1 = k["accepting"];
            }
        }

        int temp = 0;
        bool stop = false;
        while(!stop){
            for(auto l : j["states"]){
                if(l["name"] == names[temp]){
                    acc2 = l["accepting"];
                }
            }

            if(acc1 != acc2){
                string comb = "";
                comb += names[i];
                comb += names[temp];
                sort(comb.begin(), comb.end());
                checker.push_back(comb);
            }

            if(temp == i){
                stop = true;
            }else{
                temp += 1;
            }
        }
    }



    string tup;
    bool end = false;

    //Transitie check
    while(!end) {
        int counter = 0;
        for (auto i: j["alphabet"]) {
            for (auto k: names) {
                for (auto l: names) {
                    tup = "";
                    tup += k;
                    tup += l;
                    sort(tup.begin(), tup.end());

                    string temp2 = "";


                    if (k != l) {
                        for (auto o: j["transitions"]) {
                            if (k == o["from"] && i == o["input"]) {
                                temp2 += o["to"];
                            }
                            if (l == o["from"] && i == o["input"]) {
                                temp2 += o["to"];
                            }
                        }
                        sort(temp2.begin(), temp2.end());

                        int erin = 0;
                        for(auto p : checker){
                            if(tup == p){
                                erin = 1;
                            }
                        }
                        if(erin == 0) {
                            for (auto p: checker) {
                                if (temp2 == p) {
                                    checker.push_back(tup);
                                    counter += 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        if(counter == 0){
            end = true;
        }
    }



    //print
    for(int i = 1; i < j["states"].size();i++) {
        cout << i << "   ";

        int tempor = 0;
        bool cat = false;
        while (!cat) {
            int ctr = 0;

            tup = "";
            tup += names[i];
            tup += names[tempor];
            sort(tup.begin(), tup.end());

            for (auto h: checker) {
                if (tup == h) {
                    cout << "X" << "   ";
                    ctr += 1;
                    break;
                }
            }

            if (ctr == 0) {
                cout << "-" << "   ";
            }

            tempor += 1;
            if(i == tempor){
                cat = true;
            }
        }
        cout << endl;
    }

    cout << "    ";
    for(int qr = 0; qr < names.size()-1;qr++){
        cout << names[qr] << "   ";
    }

    cout << endl;
}

