project(tactile-tiled-tmx-format CXX)

find_package(pugixml CONFIG REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
