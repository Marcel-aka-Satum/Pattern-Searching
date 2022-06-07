//
// Created by Yassine on 7/06/2022.
//

#ifndef PATTERN_SEARCHING_DFATEST_H
#define PATTERN_SEARCHING_DFATEST_H
#include "../DFA.h"
#include "../gtest/include/gtest/gtest.h"

class DFAtest : public ::testing::Test{};

TEST_F(DFAtest,test_variable){
    DFA* dfa1 = new DFA;
    ASSERT_EQ("", dfa1->getReg());
    ASSERT_EQ(0, dfa1->getStates());
    ASSERT_EQ(0, dfa1->getAcceptingStates());
    ASSERT_EQ(0, dfa1->getAlphabet());
    ASSERT_EQ("", dfa1->getType());
}

TEST_F(DFAtest, testProperlyInitialized){
    DFA* dfa1 = new DFA;
    ASSERT_EQ(true, dfa1->properlyInitialized());
    DFA* dfa2 = new DFA;
    dfa1->set_initCheck(dfa2);
    bool temp = false;
    ASSERT_EQ(temp, dfa1->properlyInitialized());
}



#endif //PATTERN_SEARCHING_DFATEST_H
