cmake_minimum_required(VERSION 3.21)
project(Pattern-Searching)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

link_directories(gtest/include)

link_directories(gtest/lib)


set(RELEASE_SOURCE_FILES
        RE.cpp DFA.cpp ENFA.cpp NFA.cpp PatternSearching.cpp json.hpp)
set(DEBUG_SOURCE_FILES testMain.cpp ${RELEASE_SOURCE_FILES} test/REtest.h contract.h)

add_executable(PS main.cpp ${RELEASE_SOURCE_FILES} )

add_executable(PS_debug  ${DEBUG_SOURCE_FILES} )

target_link_libraries(PS_debug gtest)