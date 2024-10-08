project(tactile-plugins-zstd CXX)

find_package(zstd CONFIG REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
