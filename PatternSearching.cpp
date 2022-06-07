#include "PatternSearching.h"
#include "RE.h"
#include "utils.h"

PatternSearching::PatternSearching(string b, string re) {
    /*
    constructor voor wanneer de user 1 regex mee geeft.
    */
    REQUIRE(b != "", "de naam van de tesktbestand mag niet leeg zijn");
    REQUIRE(re != "", "de eerste regex mag niet leeg zijn");
    regex1 = std::move(re);
    tekstBestand = std::move(b);
    _initCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

PatternSearching::PatternSearching(string b, string re1, string re2, bool u) {
    /*
    constructor voor wanneer de user 2 regexen mee geeft.
    */
    REQUIRE(b != "", "de naam van de tesktbestand mag niet leeg zijn");
    REQUIRE(re1 != "", "de eerste regex mag niet leeg zijn");
    REQUIRE(re2 != "", "de 2de regex mag niet leeg zijn");
    regex1 = std::move(re1);
    regex2 = std::move(re2);
    tekstBestand = std::move(b);
    UorD = u;
    _initCheck = this;
    ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}

bool PatternSearching::properlyInitialized() {
    /*
     checkt of de aangeroepen functie juist is geinitialiseerd.
     */
    return _initCheck == this;
}

string PatternSearching::getRegex1() {
    /*
    returned de eerste regex
     */
    REQUIRE(this->properlyInitialized(),"Kruispunt wasn't initialized when calling getRegex1");
    return regex1;
}

string PatternSearching::getRegex2() {
    /*
     returned de de regex
     */
    REQUIRE(this->properlyInitialized(),"Kruispunt wasn't initialized when calling getRegex2");
    return regex2;
}

string PatternSearching::getTekstBestand() {
    /*
     returned de naam van het tekstbestand
     */
    REQUIRE(this->properlyInitialized(),"Kruispunt wasn't initialized when calling getTekstBestand");
    return tekstBestand;
}

bool PatternSearching::getUnieOrFalse(){
    /*
     returned false voor unie of true voor doorsnede
     */
    REQUIRE(this->properlyInitialized(),"Kruispunt wasn't initialized when calling getUnieOrFalse");
    return UorD;
}

void PatternSearching::search() {
    /*
     in deze functie zoek ik de pattern in de tekst dat de we krijgen en return ik de positie waar de patterns zijn.
     */
    REQUIRE(this->properlyInitialized(),"Kruispunt wasn't initialized when calling search");
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
                if (!word.empty()) {
                    if (minDfa.accepts(word)) {
                        string positie = "(" + to_string(x) + ", " + to_string(y) + ")";
                        posities.push_back(positie);
                        x += 1;
                    } else {
                        x += 1;
                    }
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
                if (!word.empty()) {
                    if (product.productAutomaat().accepts(word)) {
                        string positie = "(" + to_string(x) + ", " + to_string(y) + ")";
                        posities.push_back(positie);
                        x += 1;
                    } else {
                        x += 1;
                    }
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

void PatternSearching::testSearch() {
    /*
     in deze functie zoek ik de pattern in de tekst dat de we krijgen en return ik de positie in een txt bestand.
     dit gebruik ik bij de testen om de output file te vergelijken met de juiste output.
     */
    REQUIRE(this->properlyInitialized(),"Kruispunt wasn't initialized when calling testSearch");
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
                if (!word.empty()) {
                    if (minDfa.accepts(word)) {
                        string positie = "(" + to_string(x) + ", " + to_string(y) + ")";
                        posities.push_back(positie);
                        x += 1;
                    } else {
                        x += 1;
                    }
                }

            } while (iss);
            y+= 1;
            x = 0;
        }
        ofstream myfile;
        myfile.open("output.txt");
        for (int i = 0; i < posities.size(); ++i) {
            myfile << posities[i];
            if (i != posities.size() -1)
                myfile << ", ";
        }
        myfile.close();
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
                if (!word.empty()) {
                    if (product.productAutomaat().accepts(word)) {
                        string positie = "(" + to_string(x) + ", " + to_string(y) + ")";
                        posities.push_back(positie);
                        x += 1;
                    } else {
                        x += 1;
                    }
                }

            } while (iss);
            y+= 1;
            x = 0;
        }
        ofstream myfile;
        myfile.open("output.txt");
        for (int i = 0; i < posities.size(); ++i) {
            myfile << posities[i];
            if (i != posities.size() -1)
                myfile << ", ";
        }
        myfile.close();
    }
}
