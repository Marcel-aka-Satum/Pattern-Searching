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

TEST_F(ENFAtest, testProperlyInitialized){
    ENFA* enfa1 = new ENFA;
    ASSERT_EQ(true, enfa1->properlyInitialized());
    ENFA* enfa2 = new ENFA;
    enfa1->set_initCheck(enfa2);
    bool temp = false;
    ASSERT_EQ(temp, enfa1->properlyInitialized());
}

#endif //PATTERN_SEARCHING_ENFATEST_H
