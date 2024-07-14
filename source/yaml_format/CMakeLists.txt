project(tactile-yaml-format CXX)

find_package(yaml-cpp CONFIG REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
