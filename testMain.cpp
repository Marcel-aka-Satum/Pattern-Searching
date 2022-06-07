//
// Created by aaditya on 06.06.22.
//
#include "test/REtest.h"
#include "test/ENFAtest.h"
#include "gtest/include/gtest/gtest.h"
int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}