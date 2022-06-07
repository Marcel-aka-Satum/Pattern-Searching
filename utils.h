//
// Created by elias on 15.05.22.
//

#ifndef VERKEERSSIMULATIE_UTILS_H
#define VERKEERSSIMULATIE_UTILS_H

#include <iostream>

bool DirectoryExists(const std::string dirname);

bool FileExists(const std::string dirname);

bool FileIsEmpty(const std::string filename);

bool FileCompare(const std::string leftFileName, const std::string rightFileName);

#endif //VERKEERSSIMULATIE_UTILS_H
