//
// Created by aaditya on 06.05.22.
//
#include "ENFA.h"
using namespace std;

int main(){
    ENFA enfa("enfa.json");
    enfa.toDFA().print();
    return 0;
}