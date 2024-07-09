project(tactile-yaml-format CXX)

add_subdirectory(lib)

if (TACTILE_BUILD_TESTS)
  add_subdirectory(test)
endif ()
