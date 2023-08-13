cmake_minimum_required(VERSION 3.16)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

project(tactile
        HOMEPAGE_URL "https://github.com/albin-johansson/tactile"
        VERSION 0.5.0
        LANGUAGES CXX)

option(TACTILE_BUILD_APP_BUNDLE "Build as a macOS application bundle (.app)" OFF)
option(TACTILE_USE_SANITIZERS "Enable runtime sanitizers" OFF)
option(TACTILE_USE_LTO "Enable link-time optimization" OFF)
option(TACTILE_TREAT_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif ()

if (TACTILE_USE_SANITIZERS MATCHES ON)
  add_compile_options(-fno-omit-frame-pointer -fsanitize=address,undefined)
  add_link_options(-fno-omit-frame-pointer -fsanitize=address,undefined)
endif ()

set(TACTILE_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(TACTILE_VENDOR_DIR "${TACTILE_ROOT_DIR}/vendor")
set(TACTILE_ASSETS_DIR "${TACTILE_ROOT_DIR}/assets")
set(TACTILE_INCLUDE_DIR "${TACTILE_ROOT_DIR}/source/public")

# Target names
set(TACTILE_PROTO TactileProto)
set(TACTILE_LIB TactileLib)
set(TACTILE_EXE Tactile)
set(TACTILE_TEST TactileTests)

set(TACTILE_CORE tactile-core)
include(cmake/tactile.cmake)

# System dependencies
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

set(VULKAN_INCLUDE_DIR "$ENV{VULKAN_SDK}/include")

# Vcpkg dependencies
find_package(argparse CONFIG REQUIRED)
find_package(Boost REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)
find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)

find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

message(DEBUG "TACTILE_INCLUDE_DIR: ${TACTILE_INCLUDE_DIR}")
message(DEBUG "TACTILE_USE_SANITIZERS: ${TACTILE_USE_SANITIZERS}")
message(DEBUG "TACTILE_USE_LTO: ${TACTILE_USE_LTO}")
message(DEBUG "TACTILE_TREAT_WARNINGS_AS_ERRORS: ${TACTILE_TREAT_WARNINGS_AS_ERRORS}")

add_subdirectory(source/proto)
add_subdirectory(source/private)
add_subdirectory(source/lib)
add_subdirectory(source/exe)
add_subdirectory(test)