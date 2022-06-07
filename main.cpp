//
// Created by aaditya on 06.05.22.
//
#include "ENFA.h"
#include "RE.h"
#include "DFA.h"
#include "PatternSearching.h"
using namespace std;

int main(){
    RE re1("Aaditya",'e');
    ENFA enfa = re1.toENFA();
    DFA dfa = enfa.toDFA();

    RE re2 = dfa.toRE();
    ENFA enfa2 = re2.toENFA();
    DFA dfa2 = enfa2.toDFA();

    cout << boolalpha << dfa.accepts("Aaditya") << endl;
    cout << boolalpha << dfa.accepts("Aaditya") << endl;
}