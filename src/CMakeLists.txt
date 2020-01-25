file(GLOB SmallObject_SRC CONFIGURE_DEPENDS "*.cpp" "*.hpp")

add_library(
    SmallObject
    STATIC
    ${SmallObject_SRC}
)

if(BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()

if(BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif()
