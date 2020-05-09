cmake_minimum_required(VERSION 3.9)

project(helpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build configuration")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)

# add warnings to all build types
set(WARNING_FLAGS "-Wall -Wextra -Wshadow -Wdouble-promotion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS}")

# ccache to speed up builds
option(ENABLE_CCACHE "Enable ccache to speed up builds" ON)
if(ENABLE_CCACHE)
  find_program(CCACHE ccache)
  if(CCACHE)
    message(STATUS "Found ccache: ${CCACHE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CMAKE_COMMAND} -E env CCACHE_BASEDIR=${PROJECT_SOURCE_DIR} ${CCACHE})
    set(CMAKE_C_COMPILER_LAUNCHER ${CMAKE_COMMAND} -E env CCACHE_BASEDIR=${PROJECT_SOURCE_DIR} ${CCACHE})
  else()
    message(WARNING "ccache not found")
  endif()
endif()

# link time optimization/interprocedural optimization
option(ENABLE_LINK_TIME_OPTIMIZATION "Optimize during link time" ON)
if(ENABLE_LINK_TIME_OPTIMIZATION)
  cmake_policy(SET CMP0069 NEW)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT IsIpoSupported)
  if(IsIpoSupported)
      set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
      message(WARNING "IPO Not Supported")
  endif()
endif()

option(ENABLE_CODE_COVERAGE "Generate code coverage data" OFF)

# Use GTest::Main and GTest::GTest when linking libraries
find_package(GTest)
if(GTEST_FOUND)
    set(BUILD_UNIT_TESTS ON)
    include(cmake/CompileTests.cmake)
    include (CTest)
else()
    message(WARNING "GTest not found in system path. Not building unit tests.")
endif()

# use benchmark::benchmark when linking libraries
find_package(benchmark)
if(benchmark_FOUND)
    set(BUILD_BENCHMARKS ON)
else()
    message(WARNING "benchmark not found in system path. Not building benchmarks.")
endif()

if(BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

add_subdirectory(src)
