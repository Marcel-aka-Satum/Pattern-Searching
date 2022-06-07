//
// Created by aaditya on 07.06.22.
//

#ifndef PATTERN_SEARCHING_HELPER_H
#define PATTERN_SEARCHING_HELPER_H
#include "iostream"
#include "string"
#include "sys/stat.h"
#include "fstream"

namespace Helpers {
    double stod(std::string s);

    std::string to_string(double d);


    bool DirectoryExists(const std::string &dirname);

    bool FileExists(const std::string &dirname);

}


#endif //PATTERN_SEARCHING_HELPER_H
