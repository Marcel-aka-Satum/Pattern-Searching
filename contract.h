//
// Created by aaditya on 06.06.22.
//

#ifndef PATTERN_SEARCHING_CONTRACT_H
#define PATTERN_SEARCHING_CONTRACT_H
#include "cassert"


#define REQUIRE(assertion, what) \
    if (!(assertion)) _assert (what, __FILE__, __LINE__)

#define ENSURE(assertion, what) \
    if (!(assertion)) _assert (what, __FILE__, __LINE__)

#endif //PATTERN_SEARCHING_CONTRACT_H
