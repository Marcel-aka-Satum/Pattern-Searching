#include "PatternSearching.h"

PatternSearching::PatternSearching(string &b, string &re) {
    regex1 = re;
    ifstream input(b);
    input >> tekstBestand;
}

PatternSearching::PatternSearching(string &b, string &re1, string& re2) {
    regex1 = re1;
    regex2 = re2;
    ifstream input(b);
    input >> tekstBestand;
}