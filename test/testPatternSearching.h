//
// Created by elias on 07.06.22.
//

#ifndef PATTERN_SEARCHING_TESTPATTERNSEARCHING_H
#define PATTERN_SEARCHING_TESTPATTERNSEARCHING_H
#include "../PatternSearching.h"
#include "gtest/gtest.h"

class PatternSearchingTest : public ::testing::Test{};

TEST_F(PatternSearchingTest, testVariables){
    auto* p = new PatternSearching("test1.txt","test", "test2", false);
    ASSERT_EQ("test1.txt", p->getTekstBestand());
    ASSERT_EQ("test", p->getRegex1());
    ASSERT_EQ("test2", p->getRegex2());
    ASSERT_EQ(false, p->getUnieOrFalse());
}






#endif //PATTERN_SEARCHING_TESTPATTERNSEARCHING_H
