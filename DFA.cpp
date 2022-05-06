//
// Created by aaditya on 23.04.22.
//

#include "DFA.h"
#include "iomanip"
DFA::DFA(string file) {
    ifstream input(file);
    input >> j;
    input.close();
}
void DFA::print() {
    cout << setw(4) << j << endl;
}