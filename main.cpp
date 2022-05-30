//
// Created by aaditya on 06.05.22.
//
#include "ENFA.h"
#include "RE.h"
#include "DFA.h"
using namespace std;

int main(){
    RE re("pet",'e');
    ENFA enfa = re.toENFA();
    cout << boolalpha << enfa.accepts("pet") << endl;
    enfa.print();
    DFA dfa = enfa.toDFA();
    dfa.print();
    cout << boolalpha << dfa.accepts("pet") << endl;
    DFA edfa = dfa.minimize();
    cout << boolalpha << edfa.accepts("pet") << endl;
    return 0;
}