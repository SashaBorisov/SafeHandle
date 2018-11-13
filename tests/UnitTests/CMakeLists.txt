cmake_minimum_required(VERSION 3.0.0)
project(UnitTests VERSION 0.1.0)

#include(CTest)
#enable_testing()

include_directories(../../iface)
include_directories(../../import/googletest/googletest/include)
include_directories(../../import/googletest/googletest)

set(CPPS 
    ../../import/googletest/googletest/src/gtest-all.cc
    ../../import/googletest/googletest/src/gtest_main.cc
    CriticalSection_Tests.cpp
    CoInitialize_Tests.cpp
    Event_Tests.cpp
    File_Tests.cpp
    FileMapping_Tests.cpp
    Heap_Tests.cpp
    Mutex_Tests.cpp
)

add_executable(UnitTests ${CPPS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
