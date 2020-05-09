function(set_project_warnings project_name)
  set(
    GCC_WARNINGS
      -Wall
      -Wextra
      -Wmisleading-indentation
      -Wnon-virtual-dtor
      -Wlogical-op
      -Wuseless-cast
  )

  option(ENABLE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

  if(ENABLE_WARNINGS_AS_ERRORS)
    set(GCC_WARNINGS ${GCC_WARNINGS} -Werror)
  endif()

  target_compile_options(${project_name} INTERFACE ${GCC_WARNINGS})
endfunction()