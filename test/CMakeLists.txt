set(
  libraries
  SmallObject
  GTest::Main
  GTest::GTest
  project_warnings
  code_coverage
)

set(include_dirs ${PROJECT_SOURCE_DIR}/src/)

CompileTests("${libraries}" "${include_dirs}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test")