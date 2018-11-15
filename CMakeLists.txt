cmake_minimum_required(VERSION 3.0.0)
project(SafeHandle VERSION 0.1.0)

#include(${CMAKE_CURRENT_SOURCE_DIR}/tests/UnitTests/CMakeLists.txt)

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/tests/UnitTests
                  ${CMAKE_CURRENT_SOURCE_DIR}/build/tests/UnitTests )
                #  [EXCLUDE_FROM_ALL])
