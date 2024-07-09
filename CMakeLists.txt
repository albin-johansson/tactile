cmake_minimum_required(VERSION 3.16)

option(TACTILE_BUILD_TESTS "Build the tests" OFF)
option(TACTILE_BUILD_OPENGL_RENDERER "Build the OpenGL renderer" OFF)
option(TACTILE_BUILD_VULKAN_RENDERER "Build the Vulkan renderer" OFF)
option(TACTILE_MACOS_APP_BUNDLE "Build the editor as a macOS application bundle (.app)" OFF)
option(TACTILE_UNITY_BUILD "Enable unity builds" OFF)
option(TACTILE_USE_LTO "Enable link-time optimizations" OFF)
option(TACTILE_USE_PRECOMPILED_HEADERS "Enable precompiled standard headers" ON)

if (CMAKE_SKIP_INSTALL_ALL_DEPENDENCY MATCHES "")
  set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY "ON")
endif ()

message(DEBUG "TACTILE_BUILD_TESTS: ${TACTILE_BUILD_TESTS}")
message(DEBUG "TACTILE_BUILD_OPENGL_RENDERER: ${TACTILE_BUILD_OPENGL_RENDERER}")
message(DEBUG "TACTILE_BUILD_VULKAN_RENDERER: ${TACTILE_BUILD_VULKAN_RENDERER}")
message(DEBUG "TACTILE_MACOS_APP_BUNDLE: ${TACTILE_MACOS_APP_BUNDLE}")
message(DEBUG "TACTILE_UNITY_BUILD: ${TACTILE_UNITY_BUILD}")
message(DEBUG "TACTILE_USE_LTO: ${TACTILE_USE_LTO}")
message(DEBUG "TACTILE_USE_PRECOMPILED_HEADERS: ${TACTILE_USE_PRECOMPILED_HEADERS}")

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  cmake_path(SET VCPKG_ROOT NORMALIZE "$ENV{VCPKG_ROOT}")
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

if (TACTILE_BUILD_OPENGL_RENDERER MATCHES "ON")
  list(APPEND VCPKG_MANIFEST_FEATURES "opengl")
endif ()

if (TACTILE_BUILD_VULKAN_RENDERER MATCHES "ON")
  list(APPEND VCPKG_MANIFEST_FEATURES "vulkan")
endif ()

message(DEBUG "CMAKE_SKIP_INSTALL_ALL_DEPENDENCY: ${CMAKE_SKIP_INSTALL_ALL_DEPENDENCY}")
message(DEBUG "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(DEBUG "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")
message(DEBUG "VCPKG_MANIFEST_FEATURES: ${VCPKG_MANIFEST_FEATURES}")

project(tactile
        HOMEPAGE_URL "https://github.com/albin-johansson/tactile"
        VERSION 0.5.0
        LANGUAGES CXX)

# Determine build type, e.g. "debug" or "release".
string(TOLOWER "${CMAKE_BUILD_TYPE}" TACTILE_BUILD_TYPE)
if (NOT (TACTILE_BUILD_TYPE MATCHES "debug|release|asan"))
  message(FATAL_ERROR "Unsupported build type")
else ()
  message(DEBUG "TACTILE_BUILD_TYPE: ${TACTILE_BUILD_TYPE}")
endif ()

set(TACTILE_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(TACTILE_BUILD_DIR "${TACTILE_ROOT_DIR}/build/${TACTILE_BUILD_TYPE}")
set(TACTILE_VENDOR_DIR "${TACTILE_ROOT_DIR}/vendor")
set(TACTILE_ASSET_DIR "${TACTILE_ROOT_DIR}/assets")

message(DEBUG "TACTILE_BUILD_DIR: ${TACTILE_BUILD_DIR}")
message(DEBUG "TACTILE_ROOT_DIR: ${TACTILE_ROOT_DIR}")
message(DEBUG "TACTILE_VENDOR_DIR: ${TACTILE_VENDOR_DIR}")
message(DEBUG "TACTILE_ASSET_DIR: ${TACTILE_ASSET_DIR}")

install(DIRECTORY "${TACTILE_ASSET_DIR}" DESTINATION "${TACTILE_BUILD_DIR}")

include("${TACTILE_ROOT_DIR}/cmake/tactile.cmake")
tactile_find_dependencies()

add_subdirectory("source/proto")
add_subdirectory("source/base")
add_subdirectory("source/core")
add_subdirectory("source/runtime")
add_subdirectory("source/main")

add_subdirectory("source/zlib_compression")
add_subdirectory("source/zstd_compression")

add_subdirectory("source/yaml_format")
add_subdirectory("source/tiled_tmj_format")
add_subdirectory("source/tiled_tmx_format")

if (TACTILE_BUILD_OPENGL_RENDERER)
  add_subdirectory("source/opengl_renderer")
endif ()

if (TACTILE_BUILD_VULKAN_RENDERER)
  add_subdirectory("source/vulkan_renderer")
endif ()
