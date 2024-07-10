project(tactile-tiled-tmj-format CXX)

find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
