#include "PatternSearching.h"
#include "RE.h"

PatternSearching::PatternSearching(string b, string re) {
    regex1 = std::move(re);
    tekstBestand = std::move(b);
}

PatternSearching::PatternSearching(string b, string re1, string re2, bool u) {
    regex1 = std::move(re1);
    regex2 = std::move(re2);
    tekstBestand = std::move(b);
    UorD = u;
}

void PatternSearching::search() {
    if (regex2.empty()){
        RE re(regex1, 'e');
        ENFA enfa = re.toENFA();
        DFA dfa = enfa.toDFA();
        DFA minDfa = dfa.minimize();
        ifstream file;
        file.open(tekstBestand);
        if (!file.is_open()) return;
        string line;
        vector<string> posities;
        int x = 0;
        int y = 0;
        while (getline(file, line)){
            istringstream iss(line);
            do {
                string word;
                iss >> word;
                if (minDfa.accepts(word)){
                    string positie = "(" + to_string(x) + ", " + to_string(y) + ")";
                    posities.push_back(positie);
                }
                else{
                    x += 1;
                }

            } while (iss);
            y+= 1;
            x = 0;
        }
        for (int i = 0; i < posities.size(); ++i) {
            cout << posities[i];
            if (i != posities.size() -1)
                cout << ", ";
        }
    }
    else{
        RE re(regex1, 'e');
        RE re2(regex2, 'e');
        ENFA enfa = re.toENFA();
        ENFA enfa2 = re2.toENFA();
        DFA dfa = enfa.toDFA();
        DFA dfa2 = enfa2.toDFA();
        DFA mindfa = dfa.minimize();
        DFA mindfa2 = dfa2.minimize();
        DFA product(mindfa, mindfa2, UorD);
        ifstream file;
        file.open(tekstBestand);
        if (!file.is_open()) return;
        string line;
        vector<string> posities;
        int x = 0;
        int y = 0;
        while (getline(file, line)){
            istringstream iss(line);
            do {
                string word;
                iss >> word;
                if (product.productAutomaat().accepts(word)){
                    string positie = "(" + to_string(x) + ", " + to_string(y) + ")";
                    posities.push_back(positie);
                    x+=1;
                }
                else{
                    x += 1;
                }

            } while (iss);
            y+= 1;
            x = 0;
        }
        for (int i = 0; i < posities.size(); ++i) {
            cout << posities[i];
            if (i != posities.size() -1)
                cout << ", ";
        }
    }
}
