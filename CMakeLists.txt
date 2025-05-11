cmake_minimum_required(VERSION 3.30)

option(TACTILE_BUILD_TESTS "Build test targets" OFF)
option(TACTILE_BUILD_EDITOR "Build the editor application" ON)
option(TACTILE_USE_LTO "Enable link-time optimizations" OFF)
option(TACTILE_USE_CLION_IMPORT_STD_WORKAROUND "Enable CLion 'import std;' workaround" OFF)

message(DEBUG "TACTILE_BUILD_TESTS: ${TACTILE_BUILD_TESTS}")
message(DEBUG "TACTILE_BUILD_EDITOR: ${TACTILE_BUILD_EDITOR}")
message(DEBUG "TACTILE_USE_LTO: ${TACTILE_USE_LTO}")
message(DEBUG "TACTILE_USE_CLION_IMPORT_STD_WORKAROUND: ${TACTILE_USE_CLION_IMPORT_STD_WORKAROUND}")

if (CMAKE_SKIP_INSTALL_ALL_DEPENDENCY MATCHES "")
  set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)
endif ()

message(DEBUG "CMAKE_SKIP_INSTALL_ALL_DEPENDENCY: ${CMAKE_SKIP_INSTALL_ALL_DEPENDENCY}")

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  cmake_path(SET VCPKG_ROOT NORMALIZE "$ENV{VCPKG_ROOT}")
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

message(DEBUG "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

if (TACTILE_BUILD_TESTS)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif ()

if (TACTILE_BUILD_EDITOR)
  list(APPEND VCPKG_MANIFEST_FEATURES "editor")
endif ()

message(DEBUG "VCPKG_MANIFEST_FEATURES: ${VCPKG_MANIFEST_FEATURES}")
message(DEBUG "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

project(tactile
        HOMEPAGE_URL "https://github.com/albin-johansson/tactile"
        VERSION 0.5.0
        LANGUAGES CXX
        )

set(CMAKE_CXX_MODULE_STD ON)

# Determine build type, e.g. "debug" or "release".
string(TOLOWER "${CMAKE_BUILD_TYPE}" TACTILE_BUILD_TYPE)
if (NOT (TACTILE_BUILD_TYPE MATCHES "debug|release|asan"))
  message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif ()

set(TACTILE_BUILD_DIR "${PROJECT_SOURCE_DIR}/build/${TACTILE_BUILD_TYPE}")
message(DEBUG "TACTILE_BUILD_TYPE: ${TACTILE_BUILD_TYPE}")
message(DEBUG "TACTILE_BUILD_DIR: ${TACTILE_BUILD_DIR}")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/data" DESTINATION "${TACTILE_BUILD_DIR}")

include("tools/cmake/tactile.cmake")

add_subdirectory("src/tactile")

if (TACTILE_BUILD_EDITOR)
  add_subdirectory("src/tactile_editor")
  add_subdirectory("src/tactile_app")
endif ()

if (TACTILE_BUILD_TESTS)
  add_subdirectory("tests/tactile")

  if (TACTILE_BUILD_EDITOR)
    add_subdirectory("tests/tactile_editor")
  endif ()
endif ()
