//
// Created by aaditya on 07.06.22.
//

#ifndef PATTERN_SEARCHING_OUTPUTTEST_H
#define PATTERN_SEARCHING_OUTPUTTEST_H
#include "../gtest/include/gtest/gtest.h"
#include "../helper.h"
#include "../RE.h"
#include "../ENFA.h"
#include "../DFA.h"
#include "fstream"
#include "sys/stat.h"
class outputTest : public ::testing::Test{};

TEST_F(outputTest, jsonOutput){
    ASSERT_TRUE(DirectoryExists("../outputs"));
    RE re1("Aaditya", 'e');
    ENFA enfa = re1.toENFA();
    DFA dfa = enfa.toDFA();
    RE re2 = dfa.toRE();
    string regex = re2.getRegex();

    ASSERT_TRUE(FileExists("../outputs/dfa-output.json"));
    ASSERT_TRUE(FileExists("../outputs/enfa-output.json"));
    ASSERT_TRUE(FileExists("../outputs/mindfa.json"));
    ASSERT_TRUE(FileExists("../outputs/regex.txt"));
    string ss = "";
    ifstream file("../outputs/regex.txt");
    file >> ss;
    file.close();


    EXPECT_TRUE(FileCompare("../outputs/dfa-output.json","../dfa-output.json"));
    EXPECT_TRUE(FileCompare("../outputs/enfa-output.json","../enfa-output.json"));
    EXPECT_TRUE(FileCompare("../outputs/mindfa.json","../tfa.json"));
    ASSERT_EQ(ss,regex);

}

#endif //PATTERN_SEARCHING_OUTPUTTEST_H
