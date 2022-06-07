//
// Created by aaditya on 06.06.22.
//

#ifndef PATTERN_SEARCHING_ENFATEST_H
#define PATTERN_SEARCHING_ENFATEST_H
#include "../ENFA.h"
#include "../gtest/include/gtest/gtest.h"

class ENFAtest : public ::testing::Test{};

TEST_F(ENFAtest,test_variable){
    ENFA* enfa1 = new ENFA;

    ASSERT_EQ("",enfa1->getEps());

    ASSERT_EQ(0,enfa1->getAllStates());


}
#endif //PATTERN_SEARCHING_ENFATEST_H
