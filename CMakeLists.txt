cmake_minimum_required(VERSION 3.21)
project(Pattern-Searching)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Pattern-Searching
        main.cpp
        DFA.cpp
        DFA.h
        NFA.cpp
        NFA.h
        ENFA.cpp
        ENFA.h
        RE.cpp
        RE.h)