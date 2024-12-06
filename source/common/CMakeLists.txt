project("tactile-common-project" CXX)

find_package("FastFloat" CONFIG REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  find_package("GTest" CONFIG REQUIRED)
  add_subdirectory("test")
endif ()
