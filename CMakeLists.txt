cmake_minimum_required(VERSION 3.21)
project(Pattern-Searching)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS -pthread)

include_directories(gtest/include)

link_directories(gtest/lib)


set(RELEASE_SOURCE_FILES
        RE.cpp DFA.cpp ENFA.cpp NFA.cpp PatternSearching.cpp helper.cpp)
set(DEBUG_SOURCE_FILES testMain.cpp ${RELEASE_SOURCE_FILES} test/REtest.h test/ENFAtest.h test/outputTest.h test/DFAtest.h contract.h)

add_executable(PS main.cpp ${RELEASE_SOURCE_FILES} )

add_executable(PS_debug  ${DEBUG_SOURCE_FILES})

target_link_libraries(PS_debug gtest)