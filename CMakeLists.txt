cmake_minimum_required(VERSION 3.21)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  cmake_path(SET VCPKG_ROOT NORMALIZE $ENV{VCPKG_ROOT})
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

message(DEBUG "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(DEBUG "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

project(tactile
        HOMEPAGE_URL "https://github.com/albin-johansson/tactile"
        VERSION 0.5.0
        LANGUAGES CXX)

message(DEBUG "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(DEBUG "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(DEBUG "CMAKE_PROJECT_VERSION: ${CMAKE_PROJECT_VERSION}")
message(DEBUG "CMAKE_UNITY_BUILD: ${CMAKE_UNITY_BUILD}")

option(TACTILE_BUILD_APP_BUNDLE "Build the editor as a macOS application bundle (.app)" OFF)
option(TACTILE_BUILD_TESTS "Build the test suite" OFF)
option(TACTILE_BUILD_VULKAN_RENDERER "Build the Vulkan renderer component" OFF)
option(TACTILE_BUILD_LEGACY "Build the legacy targets" OFF)
option(TACTILE_UNITY_BUILD "Enable unity builds" ON)
option(TACTILE_USE_LTO "Enable link-time optimizations" OFF)

message(DEBUG "TACTILE_BUILD_APP_BUNDLE: ${TACTILE_BUILD_APP_BUNDLE}")
message(DEBUG "TACTILE_BUILD_TESTS: ${TACTILE_BUILD_TESTS}")
message(DEBUG "TACTILE_BUILD_VULKAN_RENDERER: ${TACTILE_BUILD_VULKAN_RENDERER}")
message(DEBUG "TACTILE_BUILD_LEGACY: ${TACTILE_BUILD_LEGACY}")
message(DEBUG "TACTILE_UNITY_BUILD: ${TACTILE_UNITY_BUILD}")
message(DEBUG "TACTILE_USE_LTO: ${TACTILE_USE_LTO}")

# Determine build type, e.g. "debug" or "release".
string(TOLOWER ${CMAKE_BUILD_TYPE} TACTILE_BUILD_TYPE)
if (NOT (TACTILE_BUILD_TYPE MATCHES "debug|release|asan"))
  message(FATAL_ERROR "Unsupported build type")
endif ()

message(DEBUG "TACTILE_BUILD_TYPE: ${TACTILE_BUILD_TYPE}")

set(TACTILE_CXX_STANDARD 23)

set(TACTILE_BUILD_DIR "${PROJECT_SOURCE_DIR}/build/${TACTILE_BUILD_TYPE}")
set(TACTILE_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(TACTILE_VENDOR_DIR "${TACTILE_ROOT_DIR}/vendor")
set(TACTILE_ASSET_DIR "${TACTILE_ROOT_DIR}/assets")
set(TACTILE_APP_DIR "${PROJECT_SOURCE_DIR}/source/app")
set(TACTILE_LIB_DIR "${PROJECT_SOURCE_DIR}/source/lib")

message(DEBUG "TACTILE_BUILD_DIR: ${TACTILE_BUILD_DIR}")
message(DEBUG "TACTILE_ROOT_DIR: ${TACTILE_ROOT_DIR}")
message(DEBUG "TACTILE_VENDOR_DIR: ${TACTILE_VENDOR_DIR}")
message(DEBUG "TACTILE_ASSET_DIR: ${TACTILE_ASSET_DIR}")
message(DEBUG "TACTILE_APP_DIR: ${TACTILE_APP_DIR}")
message(DEBUG "TACTILE_LIB_DIR: ${TACTILE_LIB_DIR}")

find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)
find_package(argparse CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(FastFloat CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")

install(DIRECTORY DESTINATION "${TACTILE_BUILD_DIR}/plugins")
install(DIRECTORY "${TACTILE_ASSET_DIR}" DESTINATION "${TACTILE_BUILD_DIR}")

add_subdirectory(modules)

if (TACTILE_BUILD_LEGACY MATCHES ON)
  # Target names
  set(TACTILE_PROTO TactileProto)
  set(TACTILE_LIB TactileLib)
  set(TACTILE_EXE Tactile)
  set(TACTILE_TEST TactileTests)

  add_subdirectory(source/proto)
  add_subdirectory(source/lib)
  add_subdirectory(source/app)

  if (TACTILE_BUILD_TESTS MATCHES ON)
    add_subdirectory(test)
  endif ()
endif ()
