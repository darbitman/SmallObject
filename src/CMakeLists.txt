file(GLOB SmallObject_SRC CONFIGURE_DEPENDS "*.cpp" "*.hpp")

add_library(
    SmallObject
    STATIC
    ${SmallObject_SRC}
)

add_subdirectory(test)
