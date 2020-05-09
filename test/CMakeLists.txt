set(
  libraries
  SmallObject
  GTest::Main
  GTest::GTest
  pthread
  project_warnings
)

if(ENABLE_CODE_COVERAGE)
  list(APPEND libraries gcov "--coverage")
  set(additional_compiler_flags -fprofile-arcs -ftest-coverage)
endif()

set(include_dirs ${PROJECT_SOURCE_DIR}/src/)


CompileTests("${libraries}" "${include_dirs}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test" "${additional_compiler_flags}")