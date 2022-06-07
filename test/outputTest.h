//
// Created by aaditya on 07.06.22.
//

#ifndef PATTERN_SEARCHING_OUTPUTTEST_H
#define PATTERN_SEARCHING_OUTPUTTEST_H
#include "../gtest/include/gtest/gtest.h"
#include "../helper.h"

class outputTest : public ::testing::Test{};

TEST_F(outputTest, jsonOutput){
    ASSERT_TRUE(Helpers::DirectoryExists("outputs"));
}

#endif //PATTERN_SEARCHING_OUTPUTTEST_H
