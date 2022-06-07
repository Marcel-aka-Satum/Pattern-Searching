#include "DFA.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "RE.h"
#include "contract.h"

using namespace std;

using json = nlohmann::json;

bool DFA::properlyInitialized() {
    return _initCheck == this;
}

DFA::DFA() {
    _initCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}


// common functions
DFA::DFA(const string& dfa){
    _initCheck = this;
    REQUIRE(dfa.size() > 0, "dfa string is leeg");
    ifstream input(dfa);
    input >> j;
    type = j["type"]; // Get type

    // Create alphabet
    for (string c : j["alphabet"]) {
        alphabet.push_back(c[0]);
    }
    // Initialise states
    for (auto state : j["states"]) {
        State* s = new State;
        s->name = state["name"];
        // Accepting state
        if (state["accepting"]) {
            // Add to the accepting states vector
            acceptingStates.push_back(s);
            s->accepting = true;
        }
        // Starting state
        if (state["starting"]) {
            // Set the start state
            startState = s;
            s->starting = true;
        }
        // Store state
        states.push_back(s);
//        cout << "New State: " << s->name << endl;
    }
    // Initialise state transitions
    for (auto trans : j["transitions"]) {
        State* sPtr = nullptr;
        State* ePtr = nullptr;
        // Get correct states for transition
        for (auto state : states) {
            // Check whether names correspond
            if (state->name == trans["from"]) sPtr = state; // Set the right pointer
            if (state->name == trans["to"]) ePtr = state;
        }
        // Get transition character
        string symb = trans["input"];
        bool inAlpha = false;
        // Check whether transition character in alphabet
        for (char c : alphabet) {
            if (symb[0] == c) { inAlpha = true; break; }
        }
        if (!inAlpha) { // Transition character not in alphabet
            cerr << "Transition on character '" << symb << "' not in alphabet" << endl;
            continue;
        }
        if (sPtr->transitions.find(symb[0]) == sPtr->transitions.cend()) {
            // No transitions on character exist for begin state
            vector<State*> v; v.push_back(ePtr);
            // Add character and transition state to transition map begin state
            sPtr->transitions.insert({symb[0], v});
        } else {
            // Transitions on character already exist for begin state
            // Add destination state to transition character in transitions of begin state
            sPtr->transitions[symb[0]].push_back(ePtr);
        }
//        cout << "Transition from " << sPtr->name << " to " << ePtr->name << " on " << symb << endl;
    }
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}
void DFA::print(){
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling print");
    REQUIRE(j.empty() == false, "json j is leeg");
    cout << setw(4) << j << endl;
}
bool DFA::accepts(const string &s) {
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling accepts");
    REQUIRE(s.size() > 0, "de string mag niet leeg zijn.");
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
    _initCheck = this;
    j = std::move(v);
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

DFA::DFA(const DFA &a, const DFA &b, bool c) {
    _initCheck = this;
    dfa1 = a.j;
    dfa2 = b.j;
    doorsnede_unie = c;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

DFA DFA::productAutomaat() {
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling productAutomaat");
    REQUIRE(dfa1.empty() == false, "dfa1 heeft geen informatie");
    REQUIRE(dfa2.empty() == false, "dfa2 heeft geen informatie");
    REQUIRE(doorsnede_unie == true or doorsnede_unie == false, "doorsnede_unie heeft geen bool waarde meegekregen");

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
        for (auto i: dfa1["alphabet"]) {
            bool accepting2 = false;
            newTo = "(";
            string temp;
            int ctr = 0;
            for (auto k: dfa1["transitions"]) {

                temp = "";
                temp += start[0];
                if (k["from"] == temp and i == k["input"]) {
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
                temp = "";
                temp += start[1];
                if (k["from"] == temp and i == k["input"]) {
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

//function tfa
DFA DFA::minimize() {
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling minimize");

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
                for (auto k: i) {
                    newComb = "";
                    string temp = "";
                    temp += k;
                    zz.push_back(temp);
                    hala += k;
                    for (auto l: j["transitions"]) {
                        if (temp == l["from"] and counter2 == 0 and l["input"] == a) {
                            newComb += l["to"];
                            ff.push_back(newComb);
                            counter2 += 1;
                        } else if (temp == l["from"] and counter2 == 1 and l["input"] == a) {
                            newComb += l["to"];
                            ff.push_back(newComb);

                        }
                    }


                }
                //hier halen wij de states uit arr1 en zetten wij ze in arr2.
                for (auto k: arr2) {
                    int counter3 = 0;
                    for (auto l: k) {
                        string vartje = "";
                        vartje += l;
                        for (auto op: ff) {
                            string vartje1 = "";
                            vartje1 += op;
                            if (vartje == vartje1) {
                                counter3 += 1;
                                break;
                            }
                        }
                    }
                    if (counter3 == 2) {
                        arr2.push_back(zz);
                        arr1[index].clear();
                        break;

                    }
                }
            }

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
                        }
                    }
                }

            }
            if (erin == 1) {
                tempo = pp[0];
                erin = 0;
            }
            index2 += 1;
        }

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

    //dfa states
    bool accept;
    bool start;
    for(auto i : finalVector) {
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
    }

    //dfa transitions
    for(auto i : finalVector) {
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

vector<State*> DFA::sortStates(const vector<State *> &states){
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling sortState");
    vector<State*> sorted; // Sorted part
    vector<State*> unsorted = states; // Unsorted part
    // While not everything sorted
    while (sorted.size() != states.size()) {
        // Set lowest state to first state
        State* min = unsorted[0];
        int index = 0;
        // Loop over unsorted states
        for (int i = 0; i < unsorted.size(); ++i) {
            // States comes before current min state
            if (unsorted[i]->name < min->name) {
                // Change min state
                min = unsorted[i];
                index = i;
            }
        }
        // Add min state to sorted vector
        sorted.push_back(min);
        // Clear min state from unsorted vector
        unsorted.erase(unsorted.begin() + index);
    }
    return sorted;
}
RE DFA::toRE() {
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling toRE");
    string regexString;
    for (auto accState : acceptingStates) {
        // Vector to store all states
        vector<State*> elimStates = sortStates(states);
        map<State*, map<State*, string>> transitionMap;
        for (auto state : states) {
            map<State *, string> desMap;
            if (state->name != "{}") {
                for (auto const &trans: state->transitions) {
                    string c;
                    c = trans.first;
                    for (auto desState: trans.second) {
                        if (desState->name != "{}") {
                            if (desMap.find(desState) == desMap.cend())
                                desMap.insert(make_pair(desState, c));
                            else
                                desMap[desState] = desMap[desState] + "+" + c;
                        }
                    }
                }
            }
            if (!desMap.empty())
                transitionMap.insert(make_pair(state, desMap));
        }

        bool onlyAccepting = false;

        while (!onlyAccepting) {
            onlyAccepting = true;
            for (auto state : elimStates) {
                if ((state != startState) && (state != accState))
                    onlyAccepting = false;
            }
            if (onlyAccepting)
                break;

            int i = 0;
            while (elimStates[i] == startState || elimStates[i] == accState) {
                i += 1;
            }

            bool selfTrans = false;
            string selfTransStr;
            State *elimState = elimStates[i];
            if (transitionMap[elimState].find(elimState) != transitionMap[elimState].cend()) {
                selfTrans = true;
                selfTransStr = transitionMap[elimState][elimState];
            }

            map<State *, map<State *, string>> tempMap;
            for (auto set : transitionMap) {
                if (set.first == elimState)
                    continue;
                map<State *, string> tempTransMap;
                for (auto tState : set.second) {
                    if (tState.first == elimState) {
                        for (auto elimStateTrans : transitionMap[elimState]) {
                            if (elimStateTrans.first == elimState)
                                continue;
                            string reTrans = tState.second;
                            if (selfTrans)
                                reTrans += selfTransStr + "*";
                            reTrans += elimStateTrans.second;
                            if (elimStateTrans.first == set.first) {
                                reTrans.insert(reTrans.begin(), '(');


                            }
                            if (tempTransMap.find(elimStateTrans.first) == tempTransMap.cend())
                                tempTransMap.insert(make_pair(elimStateTrans.first, reTrans));
                            else
                                tempTransMap[elimStateTrans.first] =
                                        "(" +  tempTransMap[elimStateTrans.first] + "+" + reTrans + ")";
                        }
                    } else {
                        tempTransMap.insert(make_pair(tState.first, tState.second));
                    }
                }
                if (!tempTransMap.empty())
                    tempMap.insert(make_pair(set.first, tempTransMap));

            }
            transitionMap = tempMap;

            elimStates.erase(elimStates.begin() + i);
        }

        string R = transitionMap[startState][startState];
        string S = transitionMap[startState][accState];
        string U = transitionMap[accState][accState];
        string T = transitionMap[accState][startState];


        string regex;

        if ((!T.empty() && !S.empty()) || !R.empty()) {
            if (!S.empty() && (!T.empty() || !R.empty()))
                regex += '(';
            if (!R.empty() && !S.empty()) {
                if (!T.empty())
                    regex += '(';
                regex += R;
                if (!T.empty())
                    regex += ")+";
            }
            if (!S.empty() && !T.empty()) {
                regex += '(' + S + ')';
                if (!U.empty())
                    regex += '(' + U + ")*";
                regex += '(' + T + ')';
            }
            if (!S.empty() && (!T.empty() || !R.empty()))
                regex += ")*";
        }
        if (!S.empty()) {
            regex +=  S ;
        }
        if (!U.empty() && !S.empty())
            regex += '(' +  U + ")*";

        if (regexString.empty())
            regexString = regex;
        else if (!regex.empty()) {
            regexString += "+" + regex;
        }
    }

    RE re(regexString, 'e');
    return re;

}

DFA::~DFA() {
    for (auto state : states) {
        delete state;
    }
}


const string &DFA::getReg(){
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling getReg");
    return reg;
}

unsigned long long DFA::getStates(){
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling getReg");
    return states.size();
}

unsigned long long DFA::getAcceptingStates(){
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling getReg");
    return acceptingStates.size();
}

State *DFA::getStartState(){
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling getStartState");
    return startState;
}

unsigned long long DFA::getAlphabet(){
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling getAlphabet");
    return alphabet.size();
}

const string &DFA::getType(){
    REQUIRE(this->properlyInitialized(),"DFA wasn't initialized when calling getType");
    return type;
}


DFA *DFA::get_initCheck() {
    REQUIRE(this->properlyInitialized(), "DFA wasn't initialized when calling get_initCheck");
    return _initCheck;
}

void DFA::set_initCheck(DFA* i) {
    REQUIRE(this->properlyInitialized(), "DFA wasn't initialized when calling set_initCheck");
    _initCheck = i;
}
