//
// Created by aaditya on 07.06.22.
//

#ifndef PATTERN_SEARCHING_HELPER_H
#define PATTERN_SEARCHING_HELPER_H
#include "iostream"
#include "string"
#include "sys/stat.h"
#include "fstream"
using namespace std;


bool DirectoryExists(const std::string &dirname);

bool FileExists(const std::string &dirname);
bool FileCompare(const string leftFileName, const std::string rightFileName);


#endif //PATTERN_SEARCHING_HELPER_H
