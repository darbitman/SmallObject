file(GLOB SmallObject_SRC CONFIGURE_DEPENDS "*.cpp" "*.hpp")

add_library(
    SmallObject
    STATIC
    ${SmallObject_SRC}
)

target_link_libraries(
    SmallObject
    PRIVATE
    project_warnings
    code_coverage
)

target_include_directories(
    SmallObject
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)