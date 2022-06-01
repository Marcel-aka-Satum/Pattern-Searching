//
// Created by aaditya on 06.05.22.
//
#include "ENFA.h"
#include "RE.h"
#include "DFA.h"
#include "PatternSearching.h"
using namespace std;

int main(){
    //PatternSearching p("test.txt", "dit");
    //p.search();
    RE re("q+r",'e');
    ENFA enfa = re.toENFA();
    DFA dfa = enfa.toDFA();
    RE re2("qr", 'e');
    ENFA enfa2 = re2.toENFA();
    DFA dfa2 = enfa2.toDFA();


    cout << boolalpha << dfa.accepts("q") << endl;
    cout << boolalpha << dfa2.accepts("qr") << endl;

    /*DFA mindfa = dfa.minimize();
    DFA mindfa2 = dfa2.minimize();

    cout << boolalpha << mindfa.accepts("q") << endl;
    cout << boolalpha << mindfa2.accepts("qr") << endl;*/



    DFA product(dfa, dfa2, false);

    //mindfa.print();
    //mindfa2.print();
    //DFA mindfa = product.productAutomaat().minimize();

    product.productAutomaat();

    cout << boolalpha << product.productAutomaat().accepts("q") << endl;
    cout << boolalpha << product.productAutomaat().accepts("qr") << endl;
    return 0;
}