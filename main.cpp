//
// Created by aaditya on 06.05.22.
//
#include "ENFA.h"
#include "RE.h"
#include "DFA.h"
using namespace std;

int main(){
    RE re("(m+y)*+(e+y+m+i)s",'e');
    ENFA enfa = re.toENFA();
    cout << boolalpha << enfa.accepts("ys") << endl;
    DFA dfa = enfa.toDFA();
    cout << boolalpha << dfa.accepts("ys") << endl;
    dfa.print();
    return 0;
}