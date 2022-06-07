//
// Created by elias on 07.06.22.
//

#ifndef PATTERN_SEARCHING_TESTPATTERNSEARCHINGOUTPUT_H
#define PATTERN_SEARCHING_TESTPATTERNSEARCHINGOUTPUT_H
#include "gtest/gtest.h"
#include "../utils.h"

class OutputTest: public ::testing::Test {};

TEST_F(OutputTest, outputPatternsearching){
    ASSERT_TRUE(DirectoryExists("testOutput"));
    string fileName = "testOutput/test1.txt";
    string outputFileName = "testOutput/outputTest1.txt";

    PatternSearching p1(fileName, "(m+y)*+(y+m+i)s");
    p1.testSearch();
    EXPECT_TRUE(FileCompare("output.txt", outputFileName));
    fileName = "testOutput/test2.txt";
    outputFileName = "testOutput/outputTest2.txt";
    PatternSearching p2(fileName, "m");
    p2.testSearch();
    EXPECT_TRUE(FileCompare("output.txt", outputFileName));
    fileName = "testOutput/test3.txt";
    outputFileName = "testOutput/outputTest3.txt";
    PatternSearching p3(fileName, "wij","i",false);
    p3.testSearch();
    EXPECT_TRUE(FileCompare("output.txt", outputFileName));
    fileName = "testOutput/test4.txt";
    outputFileName = "testOutput/outputTest4.txt";
    PatternSearching p4(fileName, "het","t",false);
    p4.testSearch();
    EXPECT_TRUE(FileCompare("output.txt", outputFileName));
    fileName = "testOutput/test5.txt";
    outputFileName = "testOutput/outputTest5.txt";
    PatternSearching p5(fileName, "aa");
    p5.testSearch();
    EXPECT_TRUE(FileCompare("output.txt", outputFileName));
}


#endif //PATTERN_SEARCHING_TESTPATTERNSEARCHINGOUTPUT_H
