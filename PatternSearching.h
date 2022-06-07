#ifndef PATTERN_SEARCHING_PATTERNSEARCHING_H
#define PATTERN_SEARCHING_PATTERNSEARCHING_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include <string>
#include <utility>
#include <vector>
#include "string"
#include "contract.h"

using namespace std;
using json = nlohmann::json;

class PatternSearching {
private:
    string regex1;
    string regex2;
    string tekstBestand;
    bool UorD = false;
    PatternSearching* _initCheck;

public:
    /*
     checkt of de aangeroepen functie juist is geinitialiseerd.
     */
    bool properlyInitialized();

    /*
    constructor voor wanneer de user 1 regex mee geeft.
    */
    PatternSearching(string b, string re);

    /*
    constructor voor wanneer de user 2 regexen mee geeft.
    */
    PatternSearching(string b, string re1, string re2, bool u);

    /*
    returned de eerste regex
     */
    string getRegex1();

    /*
     returned de de regex
     */
    string getRegex2();

    /*
     returned de naam van het tekstbestand
     */
    string getTekstBestand();

    /*
     returned false voor unie of true voor doorsnede
     */
    bool getUnieOrFalse();

    /*
     in deze functie zoek ik de pattern in de tekst dat de we krijgen en return ik de positie waar de patterns zijn.
     */
    void search();

    /*
     in deze functie zoek ik de pattern in de tekst dat de we krijgen en return ik de positie in een txt bestand.
     dit gebruik ik bij de testen om de output file te vergelijken met de juiste output.
     */
    void testSearch();
};


#endif //PATTERN_SEARCHING_PATTERNSEARCHING_H
