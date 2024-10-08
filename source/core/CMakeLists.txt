project(tactile-core CXX)

find_package(Boost REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(FastFloat CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)

find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_subdirectory("lib")

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
