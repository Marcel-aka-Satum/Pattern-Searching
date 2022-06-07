//
// Created by aaditya on 07.06.22.
//

#ifndef PATTERN_SEARCHING_OUTPUTTEST_H
#define PATTERN_SEARCHING_OUTPUTTEST_H
#include "../gtest/include/gtest/gtest.h"
#include "../utils.h"
#include "../RE.h"
#include "../ENFA.h"
#include "../DFA.h"
#include "fstream"
#include "sys/stat.h"
class outputTest : public ::testing::Test{};

TEST_F(outputTest, jsonOutput){
    ASSERT_TRUE(DirectoryExists("testOutput"));
    RE re1("Aaditya", 'e');
    ENFA enfa = re1.toENFA();
    DFA dfa = enfa.toDFA();
    RE re2 = dfa.toRE();
    string regex = re2.getRegex();

    ASSERT_TRUE(FileExists("testOutput/dfa-output.json"));
    ASSERT_TRUE(FileExists("testOutput/enfa-output.json"));
    ASSERT_TRUE(FileExists("testOutput/mindfa.json"));
    ASSERT_TRUE(FileExists("testOutput/regex.txt"));
    string ss = "";
    ifstream file("testOutput/regex.txt");
    file >> ss;
    file.close();


    EXPECT_TRUE(FileCompare("testOutput/dfa-output.json","testOutput/dfa-output.json"));
    EXPECT_TRUE(FileCompare("testOutput/enfa-output.json","testOutput/enfa-output.json"));
    EXPECT_TRUE(FileCompare("testOutput/mindfa.json","testOutput/mindfa.json"));
    ASSERT_EQ(ss,regex);

}

#endif //PATTERN_SEARCHING_OUTPUTTEST_H
