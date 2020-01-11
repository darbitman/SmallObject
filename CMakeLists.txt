cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(SmallObject)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

if(${CMAKE_BUILD_TYPE} STREQUAL RELEASE)
    add_definitions(-DNDEBUG)
endif()

add_subdirectory(src)
