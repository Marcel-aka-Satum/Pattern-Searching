//
// Created by aaditya on 06.06.22.
//

#ifndef PATTERN_SEARCHING_RETEST_H
#define PATTERN_SEARCHING_RETEST_H
#include "../RE.h"
#include "../gtest/include/gtest/gtest.h"

class REtest : public ::testing::Test{};
/**
 * Test the variable
 */

TEST_F(REtest,test_variable){
    RE* re1 = new RE;
    ASSERT_EQ("", re1->getRegex());

    ASSERT_EQ(0,re1->getAlph());

    ASSERT_EQ("",re1->getEps());

    ASSERT_EQ(1,re1->getStates());
}

TEST_F(REtest, testProperlyInitialized){
    RE* re1 = new RE;
    ASSERT_EQ(true, re1->properlyInitialized());
    RE* re2 = new RE;
    re1->set_initCheck(re2);
    bool temp = false;
    ASSERT_EQ(temp, re1->properlyInitialized());
}

#endif //PATTERN_SEARCHING_RETEST_H
