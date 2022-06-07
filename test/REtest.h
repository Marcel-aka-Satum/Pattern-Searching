//
// Created by aaditya on 06.06.22.
//

#ifndef PATTERN_SEARCHING_RETEST_H
#define PATTERN_SEARCHING_RETEST_H
#include "../RE.h"
#include "../gtest/include/gtest/gtest.h"

class REtest : public ::testing::Test{};

TEST_F(REtest,test_variable){
    RE* re1 = new RE;
    ASSERT_EQ("", re1->getRegex());

    ASSERT_EQ(0,re1->getAlph());

    ASSERT_EQ("",re1->getEps());

    ASSERT_EQ(0,re1->getStates());

    //ASSERT_EQ(json)
}

#endif //PATTERN_SEARCHING_RETEST_H
