//
// Created by aaditya on 07.06.22.
//

#include "helper.h"
#include "sys/stat.h"

bool DirectoryExists(const std::string &dirname) {
    struct stat st;
    return stat(dirname.c_str(), &st) == 0;
}

// Addapted from https://github.com/sergedemeyer/TicTacToe_Git
bool FileExists(const std::string &filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0)
        return false;
    std::ifstream f(filename.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}