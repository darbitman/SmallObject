file(GLOB test_SRC CONFIGURE_DEPENDS "*.cpp")

add_executable(
    SmallObjectTests
    ${test_SRC}
)

target_link_libraries(
    SmallObjectTests
    PRIVATE
    SmallObject
    gtest_main
    gtest
    pthread
)

target_include_directories(
    SmallObjectTests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
