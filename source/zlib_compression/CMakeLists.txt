project(tactile-zlib-compression CXX)

find_package(ZLIB REQUIRED)

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
