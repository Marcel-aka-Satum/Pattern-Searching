//
// Created by aaditya on 23.04.22.
//
#include "DFA.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;

DFA::DFA(string p){
    filename = p;
    ifstream input(filename);
    input >> j;
}
void DFA::print(){
    cout << setw(4) << j << endl;
}