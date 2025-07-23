cmake_minimum_required(VERSION 3.30)

message(DEBUG "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(DEBUG "CMAKE_SKIP_INSTALL_ALL_DEPENDENCY: ${CMAKE_SKIP_INSTALL_ALL_DEPENDENCY}")
message(DEBUG "CMAKE_EXPERIMENTAL_CXX_IMPORT_STD: ${CMAKE_EXPERIMENTAL_CXX_IMPORT_STD}")
message(DEBUG "CMAKE_CXX_MODULE_STD: ${CMAKE_CXX_MODULE_STD}")
message(DEBUG "VCPKG_OVERLAY_TRIPLETS: ${VCPKG_OVERLAY_TRIPLETS}")
message(DEBUG "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

option(TACTILE_ENABLE_TESTS "Enable tests" OFF)
option(TACTILE_ENABLE_EDITOR "Enable the editor application" OFF)
option(TACTILE_ENABLE_ZLIB "Enable Zlib compression support" OFF)
option(TACTILE_ENABLE_ZSTD "Enable Zstd compression support" OFF)
option(TACTILE_ENABLE_YAML "Enable Tactile YAML format support" OFF)
option(TACTILE_ENABLE_TMJ "Enable Tiled TMJ format support" OFF)
option(TACTILE_ENABLE_TMX "Enable Tiled TMJ format support" OFF)
option(TACTILE_ENABLE_TSCN "Enable Godot TSCN format support" OFF)
option(TACTILE_ENABLE_LTO "Enable link-time optimizations" OFF)
option(TACTILE_ENABLE_CLION_IMPORT_STD_WORKAROUND "Enable CLion 'import std;' workaround" OFF)

message(DEBUG "TACTILE_ENABLE_TESTS: ${TACTILE_ENABLE_TESTS}")
message(DEBUG "TACTILE_ENABLE_EDITOR: ${TACTILE_ENABLE_EDITOR}")
message(DEBUG "TACTILE_ENABLE_ZLIB: ${TACTILE_ENABLE_ZLIB}")
message(DEBUG "TACTILE_ENABLE_ZSTD: ${TACTILE_ENABLE_ZSTD}")
message(DEBUG "TACTILE_ENABLE_YAML: ${TACTILE_ENABLE_YAML}")
message(DEBUG "TACTILE_ENABLE_TMJ: ${TACTILE_ENABLE_TMJ}")
message(DEBUG "TACTILE_ENABLE_TMX: ${TACTILE_ENABLE_TMX}")
message(DEBUG "TACTILE_ENABLE_TSCN: ${TACTILE_ENABLE_TSCN}")
message(DEBUG "TACTILE_ENABLE_LTO: ${TACTILE_ENABLE_LTO}")
message(DEBUG "TACTILE_ENABLE_CLION_IMPORT_STD_WORKAROUND: ${TACTILE_ENABLE_CLION_IMPORT_STD_WORKAROUND}")

if (TACTILE_ENABLE_TESTS)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif ()

if (TACTILE_ENABLE_EDITOR)
  list(APPEND VCPKG_MANIFEST_FEATURES "editor")
endif ()

if (TACTILE_ENABLE_ZLIB)
  list(APPEND VCPKG_MANIFEST_FEATURES "zlib")
endif ()

if (TACTILE_ENABLE_ZSTD)
  list(APPEND VCPKG_MANIFEST_FEATURES "zstd")
endif ()

if (TACTILE_ENABLE_YAML)
  list(APPEND VCPKG_MANIFEST_FEATURES "yaml")
endif ()

if (TACTILE_ENABLE_TMJ)
  list(APPEND VCPKG_MANIFEST_FEATURES "tmj")
endif ()

if (TACTILE_ENABLE_TMX)
  list(APPEND VCPKG_MANIFEST_FEATURES "tmx")
endif ()

message(DEBUG "VCPKG_MANIFEST_FEATURES: ${VCPKG_MANIFEST_FEATURES}")

project(tactile
        HOMEPAGE_URL "https://github.com/albin-johansson/tactile"
        VERSION 0.5.0
        LANGUAGES CXX
        )

# Determine build type, e.g. "debug" or "release".
string(TOLOWER "${CMAKE_BUILD_TYPE}" TACTILE_BUILD_TYPE)
if (NOT (TACTILE_BUILD_TYPE MATCHES "debug|release|asan"))
  message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif ()
message(DEBUG "TACTILE_BUILD_TYPE: ${TACTILE_BUILD_TYPE}")

set(TACTILE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build/${TACTILE_BUILD_TYPE}/output")
message(DEBUG "TACTILE_BINARY_DIR: ${TACTILE_BINARY_DIR}")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/data" DESTINATION "${TACTILE_BINARY_DIR}")

include("tools/cmake/common.cmake")

find_package(Boost REQUIRED COMPONENTS safe_numerics)
find_package(FastFloat CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_subdirectory("src/core")

if (TACTILE_ENABLE_EDITOR)
  find_package(argparse CONFIG REQUIRED)
  find_package(imgui CONFIG REQUIRED)

  add_subdirectory("src/ui")

  if (TACTILE_ENABLE_ZLIB)
    find_package(ZLIB REQUIRED)
    add_subdirectory("src/zlib")
  endif ()

  add_subdirectory("src/editor")
  add_subdirectory("src/app")
endif ()

if (TACTILE_ENABLE_TESTS)
  find_package(GTest CONFIG REQUIRED)

  add_subdirectory("tests/core")

  if (TACTILE_ENABLE_EDITOR)
    if (TACTILE_ENABLE_ZLIB)
      add_subdirectory("tests/zlib")
    endif ()

    add_subdirectory("tests/editor")
  endif ()
endif ()
