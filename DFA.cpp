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
    string beginState;
    // hier kijk ik wat mijn starting state is en zet ik dat state true en geef dat door aan de var currentState
    for(int i = 0; i < j["states"].size(); i++){
        if (j["states"][i]["starting"] == true){
            states[i] = true;
            currentState = j["states"][i]["name"];
            beginState = j["states"][i]["name"];
        }
    }
    // hier kijk ik of de char in de alphabet zit, zoniet dn zet ik de curr state nr een dead state.
    for (auto i: s) {
        string inp;
        inp += i;
        bool inAlphabet = false;
        for (const auto & l : j["alphabet"]){
            if (inp == l){
                inAlphabet = true;
                if (currentState == "DEAD STATE"){
                    currentState = beginState;
                }
            }
        }
        if (inAlphabet) {
            // hier kijk ik welke transitie het moet doen, door te kijken wat de currentState is en de input.
            for (auto k: j["transitions"]) {
                if ((k["from"] == currentState) and (k["input"] == inp)) {
                    currentState = k["to"];
                    for (int h = 0; h < j["states"].size(); h++) {
                        if (j["states"][h]["name"] == currentState) {
                            if (j["states"][h]["accepting"] == true) {
                                return true;
                            }
                        }
                    }
                    break;
                }
            }
        }
        else{
            currentState="DEAD STATE";
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
    doorsnede_unie = c;

    unieOrDoorsnede = c;
    if (c)
        doDoorsnede();
    else
        doUnie();
}

DFA DFA::productAutomaat() {
    json product;

    vector<json> states;
    vector<json> transitions;


    bool accepting = false;
    string startingState = "(";
    string nummersOnly;

    vector<string> start;
    for (auto i: dfa1["states"]) {
        if (i["starting"] == true) {
            start.push_back(i["name"]);
            nummersOnly += i["name"];
            accepting = i["accepting"];
        }

    }
    for (auto i: dfa2["states"]) {
        if (i["starting"] == true) {
            start.push_back(i["name"]);
            nummersOnly += i["name"];
            accepting = i["accepting"];
        }
    }

    int komma = 0;
    for (auto i: start) {
        if (komma > 0) {
            startingState += ",";
        }
        startingState += i;
        komma += 1;
    }
    startingState += ")";


    json stateS;
    stateS["name"] = startingState;
    stateS["starting"] = true;
    stateS["accepting"] = accepting;
    states.push_back(stateS);

    string newTo;
    int index = 0;
    bool doorgaan = true;
    while(doorgaan) {
        vector<string> qq;
        //cout << "num: " << nummersOnly << endl;
        for (auto i: dfa1["alphabet"]) {
            bool accepting2 = false;
            newTo = "(";
            string temp;
            int ctr = 0;
            for (auto k: dfa1["transitions"]) {

                temp = "";
                temp += start[0];
                //cout << "start[0]:  " << temp << endl;
                //cout << "kfrom: " << k["from"] << "    temp: " << temp << endl;
                if (k["from"] == temp and i == k["input"]) {
                    //cout << "kto1: " << k["to"] << endl;
                    //cout << "kto:  " << k["to"] << endl;
                    newTo += k["to"];
                    newTo += ",";
                    for (auto l: dfa1["states"]) {
                        if (l["name"] == k["to"]) {
                            if(l["accepting"] == true){
                                ctr += 1;
                            }

                        }
                    }
                }
            }

            for (auto k: dfa2["transitions"]) {

                //cout << "kfrom: " << k["from"] << "    num[1]: " << nummersOnly[1] << endl;
                temp = "";
                temp += start[1];
                //cout << "start[0]:  " << temp << endl;
                if (k["from"] == temp and i == k["input"]) {
                    //cout << "kto2: " << k["to"] << endl;
                    //cout << "kto:  " << k["to"] << endl;
                    newTo += k["to"];
                    for (auto l: dfa2["states"]) {
                        if (l["name"] == k["to"]) {
                            if(l["accepting"] == true){
                                ctr += 1;
                            }

                        }
                    }
                }
            }
            newTo += ")";


            json transitie;
            transitie["from"] = startingState;
            transitie["to"] = newTo;
            transitie["input"] = i;
            transitions.push_back(transitie);

            bool statesCheck = true;
            for (auto k: states) {
                if (k["name"] == newTo) {
                    statesCheck = false;
                }
            }

            if(doorsnede_unie){
                if(ctr == 2){
                    accepting2 = true;
                }
            }else{
                if(ctr >= 1){
                    accepting2 = true;
                }
            }



            if (statesCheck) {
                json state;
                state["name"] = newTo;
                state["starting"] = false;
                state["accepting"] = accepting2;
                states.push_back(state);
            }

        }



        index += 1;
        if (index == states.size()) {
            doorgaan = false;
        } else {

            string nextState = states[index]["name"];
            int counterrr = 0;
            int counterrr2 = 0;
            string tre;
            string tres;
            for(auto k : nextState){
                string var1;
                var1 += k;

                    if(var1 == "{"){
                        counterrr += 1;
                    }

                    if(counterrr >= 1){
                        if(counterrr2 == 0){
                            tre += k;
                        }
                        if(counterrr2 == 1){
                            tres += k;
                        }
                    }

                    if(var1 == "}"){
                        counterrr -= 1;

                    }

                    if(counterrr == 0 and var1 != "(" and var1 != ")"){
                        counterrr2 = 1;
                    }


            }

            qq.push_back(tre);
            qq.push_back(tres);


            nummersOnly = "";
            start.clear();
            startingState = "(";
            int komCheck = 0;
            for (auto k: qq) {
                if(komCheck == 1){
                    startingState += ",";
                }
                start.push_back(k);
                nummersOnly += k;
                startingState += k;
                komCheck += 1;
            }
            startingState += ")";

        }

    }



    product["type"] = "DFA";
    product["alphabet"] = dfa1["alphabet"];
    product["states"] = states;
    product["transitions"] = transitions;

    ofstream output("product.json");
    output << product;
    output.close();

    return DFA("product.json");

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
    //elke state naam wordt in deze vector gezet.
    vector <string> names;
    for (auto i: j["states"]) {
        names.push_back(i["name"]);
    }
    sort(names.begin(), names.end());




    //elke mogelijke combinatie van 2 states wordt hier gemaakt en gezet in een vector.
    vector <vector<string>> arr;
    vector <string> arrrr;
    for (auto i: names) {
        for (auto k: names) {
            if (i != k) {
                vector <string> tt;
                tt.push_back(i);
                tt.push_back(k);

                string comb;
                comb += i;
                comb += k;
                sort(comb.begin(), comb.end());

                int dupe = 0;
                for (auto l: arrrr) {
                    if (l == comb) {
                        dupe = 1;
                    }
                }
                if (dupe == 0) {
                    arrrr.push_back(comb);
                    sort(tt.begin(), tt.end());
                    arr.push_back(tt);
                }
            }
        }
    }



    //Hier maken wij de tabel aan zonder het te visualizeren.
    //In arr1 zijn alle combinaties waar nog geen kruisje door getrokken zijn.
    //En arr2 zijn alle combinaties waar een kruisje door getrokke is.
    vector <vector<string>> arr1;
    vector <vector<string>> arr2;

    bool acc1;
    bool acc2;

    //checken of dat een comb in arr1 moet of arr2.
    for (auto i: arr) {
        int counter = 0;
        vector <string> rr;
        string cop = "";
        for (auto k: i) {
            string temp = "";
            temp += k;
            rr.push_back(temp);
            cop += k;

            for (auto l: j["states"]) {
                if (temp == l["name"] and counter == 0) {
                    acc1 = l["accepting"];
                    counter += 1;
                } else if (temp == l["name"] and counter == 1) {
                    acc2 = l["accepting"];
                }
            }
        }
        if (acc1 == acc2) {
            arr1.push_back(rr);
        } else {
            arr2.push_back(rr);
        }
    }

    /*cout << "arr1: "<< endl;
    for(auto i : arr1){
        cout << "vector: " << endl;
        for(auto k : i){
            cout << k << endl;
        }
    }
    cout << "arr2: "<< endl;
    for(auto i : arr2){
        cout << "vector: " << endl;
        for(auto k : i){
            cout << k << endl;
        }
    }
    cout << "________________" << endl;*/


    //states uit arr1 halen(dus een kruisje geven) en in arr2 zetten a.d.h.v. de transitiefunctie.
    bool rec = true;
    while(rec){
        int index = 0;
        for (auto i: arr1) {
            //cout << "eerste loop: " << i << endl;
            int counter2 = 0;
            for (auto a: j["alphabet"]) {
                string newComb = "";
                vector <string> zz;
                vector <string> ff;
                string hala = "";
                //cout << "alphabet: " << a << endl;
                for (auto k: i) {
                    //cout << "letter van comb:  " << k << endl;
                    newComb = "";
                    string temp = "";
                    temp += k;
                    zz.push_back(temp);
                    hala += k;
                    for (auto l: j["transitions"]) {
                        //cout << "in for van trans" << endl;
                        if (temp == l["from"] and counter2 == 0 and l["input"] == a) {
                            //cout << "in  de if" << endl;
                            newComb += l["to"];
                            ff.push_back(newComb);
                            counter2 += 1;
                        } else if (temp == l["from"] and counter2 == 1 and l["input"] == a) {
                            //cout << "in  de else if" << endl;
                            newComb += l["to"];
                            ff.push_back(newComb);

                        }
                        //cout << "comb:  " << newComb << endl;
                    }


                }
                //hier halen wij de states uit arr1 en zetten wij ze in arr2.
                for (auto k: arr2) {
                    //cout << "in1" << endl;
                    int counter3 = 0;
                    //cout << "nieuwe k: " << endl;
                    for (auto l: k) {
                        //cout << "in2" << endl;
                        string vartje = "";
                        vartje += l;
                        for (auto op: ff) {
                            //cout << "in3" << endl;
                            string vartje1 = "";
                            vartje1 += op;
                            //cout << "arr2: " << vartje << "  zz: " << vartje1 << endl;
                            if (vartje == vartje1) {
                                //cout << "in4" << endl;
                                counter3 += 1;
                                break;
                            }
                        }
                    }
                    if (counter3 == 2) {
                        //cout << "in55555555555555555555" << endl;
                        //cout << "print: " << zz[0] << "    " << zz[1] << endl;
                        arr2.push_back(zz);
                        arr1[index].clear();
                        break;

                    }
                }
            }
            /*cout << "arr1: "<< endl;
            for(auto i : arr1){
                cout << "vector: " << endl;
                for(auto k : i){
                    cout << k << endl;
                }
            }
            cout << "arr2: "<< endl;
            for(auto i : arr2){
                cout << i << endl;
            }
            cout << "________________" << endl;*/

            //een variable die ik gebruik.
            index += 1;
        }
        vector<vector<string>> temporor;
        for (auto i : arr1) {
            if (i.size() > 0) {
                temporor.push_back(i);
            }
        }
        if(arr1.size() == temporor.size()){
            rec = false;
        }else{
            arr1.clear();
            for (auto i : temporor) {
                arr1.push_back(i);
            }
        }
    }


    //alle states die nog niet gekruist zijn worden in een nieuwe vector gezet.
    vector<vector<string>> finalVector;
    for (auto i : arr1) {
        finalVector.push_back(i);
    }

    /*for(auto i : finalVector){
        cout << "nieuw: " << endl;
        for(auto k : i){
            cout << "finalvectortop: " << k << endl;
        }
    }
    cout << "___________________" << endl;*/


    //alle states combineren en de single states toevoegen.

    bool stop = false;
    vector<vector<string>> aangepast;


    while(!stop and finalVector.size() > 0){
        string temp = "";
        vector<string> tempo;
        int index2 = 0;
        int erin = 1;
        for (auto i: finalVector) {
            vector <vector<string>> pp;
            if(index2 == 0) {
                pp.push_back(i);
            }
            /*string str = "";
            str += i;*/
            if (index2 > 0) {
                vector<string> hah;
                for (auto k : tempo) {
                    string var1 = "";
                    var1 += k;
                    for (auto l : i) {
                        string var2 = "";
                        var2 += l;
                        if (var1 == var2) {
                            for(auto s : tempo){
                                string var3 = "";
                                var3 += s;
                                hah.push_back(var3);
                            }
                            for(auto d : i){
                                string var4 = "";
                                var4 += d;
                                hah.push_back(var4);
                            }
                            pp.push_back(hah);
                            erin = 1;
                            /*str = "";
                            str += temp;
                            str += i;
                            sort(str.begin(), str.end());*/
                        }
                    }
                }

            }
            if (erin == 1) {
                tempo = pp[0];
                /*temp = "";
                temp += str;*/
                erin = 0;
            }
            index2 += 1;
        }

        /*for(auto i : tempo){
            cout << "tempoV: " << i << endl;
        }*/

        //Verwijderen van duplicates.
        auto end = tempo.end();
        for (auto it = tempo.begin(); it != end; ++it) {
            end = std::remove(it + 1, end, *it);
        }

        tempo.erase(end, tempo.end());



        //states uit finalVector halen
        int index3 = 0;
        for(auto i : finalVector){
            for(auto k : i){
                string var = "";
                var += k;
                for(auto l : tempo){
                    string var0 = "";
                    var0 += l;
                    if(var == var0){
                        finalVector[index3].clear();
                    }
                }
            }
            index3 += 1;
        }


        vector<vector<string>> tempor;
        for(auto i : finalVector){
            if(i.size() > 0){
                tempor.push_back(i);
            }
        }
        finalVector.clear();
        for(auto i : tempor){
            finalVector.push_back(i);
        }
        finalVector.push_back(tempo);


        if(finalVector.size() == aangepast.size()){
            stop = true;
        }
        aangepast.clear();
        for(auto i : finalVector){
            aangepast.push_back(i);
        }
    }



    //single states toevoegen.
    int ctr = 0;
    for(auto i : names){
        vector<string> bb;
        bb.push_back(i);
        string t = "";
        t += i;
        for(auto k : finalVector){
            for(auto l : k){
                string y = "";
                y += l;
                if(t == y){
                    ctr += 1;
                }
            }
        }
        if(ctr == 0){
            finalVector.push_back(bb);
        }
        ctr = 0;
    }


    vector<json> states;
    vector<json> transitions;

    /*for(auto i : finalVector){
        cout << "nieuwe vector." << endl;
        for(auto k : i){
            cout << "finalVector: " << k << endl;
        }
    }*/



    //dfa states
    bool accept;
    bool start;
    for(auto i : finalVector) {
        //for (auto mo : i) {
        for (auto k: j["states"]) {
            json state;
            string ter = "";
            ter += i[0];

            if (i.size() == 1 and k["name"] == ter) {
                accept = k["accepting"];
                start = k["starting"];

                string name = "{";
                name += ter + "}";

                state["name"] = name;
                state["accepting"] = accept;
                state["starting"] = start;
                states.push_back(state);
                break;
            } else if (i.size() > 1 and k["name"] == ter) {
                accept = k["accepting"];
                start = k["starting"];

                int countertje2 = 0;
                string name2 = "{";
                for (auto m: i) {
                    name2 += m;
                    countertje2 += 1;
                    if (countertje2 != i.size()) {
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
        //}
    }

    //dfa transitions
    for(auto i : finalVector) {
        //for (auto mo: i) {
        for (auto l: j["alphabet"]) {
            for (auto k: j["transitions"]) {
                string ter = "";
                ter += i[0];

                //single states
                if (i.size() == 1 and k["from"] == ter and k["input"] == l) {
                    json trans;
                    string name3 = "{";
                    name3 += ter + "}";
                    trans["from"] = name3;

                    string po = "";
                    po += k["to"];
                    vector <string> aa;

                    for (auto u: finalVector) {
                        for (auto p: u) {
                            string yu = "";
                            yu += p;
                            if (yu == po) {
                                aa = u;
                            }
                        }
                    }

                    int countertje4 = 0;
                    string name4 = "{";
                    for (auto m: aa) {
                        name4 += m;
                        countertje4 += 1;
                        if (countertje4 != aa.size()) {
                            name4 += ", ";
                        }
                    }
                    name4 += "}";


                    trans["to"] = name4;
                    trans["input"] = l;
                    transitions.push_back(trans);
                }
                    //combined states
                else if (i.size() > 1 and k["from"] == ter and k["input"] == l) {
                    json trans;
                    int countertje5 = 0;
                    string name5 = "{";
                    for (auto m: i) {
                        name5 += m;
                        countertje5 += 1;
                        if (countertje5 != i.size()) {
                            name5 += ", ";
                        }
                    }
                    name5 += "}";

                    trans["from"] = name5;
                    string top;
                    top += k["to"];


                    vector <string> ww;
                    for (auto h: finalVector) {
                        for (auto m: h) {
                            string tp = "";
                            tp += m;
                            if (top == tp) {
                                ww = h;
                            }
                        }
                    }

                    int countertje6 = 0;
                    string name6 = "{";
                    for (auto m: ww) {
                        name6 += m;
                        countertje6 += 1;
                        if (countertje6 != ww.size()) {
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
        //}
    }



    //de dfa aan maken en meegeven als een json file.
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
        names2.push_back(i["name"]);
    }
    sort(names2.begin(), names2.end());


    //Accepting check
    for(int i = 1; i < j["states"].size(); i++){
        bool acc1;
        bool acc2;
        for(auto k : j["states"]){
            if(k["name"] == names2[i]){
                acc1 = k["accepting"];
            }
        }

        int temp = 0;
        bool stop = false;
        while(!stop){
            for(auto l : j["states"]){
                if(l["name"] == names2[temp]){
                    acc2 = l["accepting"];
                }
            }

            if(acc1 != acc2){
                string comb = "";
                comb += names2[i];
                comb += names2[temp];
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
            tup += names2[i];
            tup += names2[tempor];
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
    for(int qr = 0; qr < names2.size()-1;qr++){
        cout << names2[qr] << "   ";
    }

    cout << endl;
}
