//
// Created by aaditya on 06.05.22.
//
#include "ENFA.h"
#include "RE.h"
#include "DFA.h"
#include "PatternSearching.h"
using namespace std;

int main(){
    PatternSearching p("test.txt","dummy", "m", false);
    p.search();
    return 0;
}