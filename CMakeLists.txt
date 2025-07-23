# Copyright (C) 2025 Albin Johansson

cmake_minimum_required(VERSION 4.0)

option(TACTILE_ENABLE_TESTS "Enable tests" OFF)
option(TACTILE_ENABLE_EDITOR "Enable the editor application" OFF)
option(TACTILE_ENABLE_ZLIB "Enable Zlib compression support" OFF)
option(TACTILE_ENABLE_ZSTD "Enable Zstd compression support" OFF)
option(TACTILE_ENABLE_YAML "Enable Tactile YAML format support" OFF)
option(TACTILE_ENABLE_TMJ "Enable Tiled TMJ format support" OFF)
option(TACTILE_ENABLE_TMX "Enable Tiled TMJ format support" OFF)
option(TACTILE_ENABLE_TSCN "Enable Godot TSCN format support" OFF)
option(TACTILE_ENABLE_LTO "Enable link-time optimizations" OFF)

message(DEBUG "TACTILE_ENABLE_TESTS: ${TACTILE_ENABLE_TESTS}")
message(DEBUG "TACTILE_ENABLE_EDITOR: ${TACTILE_ENABLE_EDITOR}")
message(DEBUG "TACTILE_ENABLE_ZLIB: ${TACTILE_ENABLE_ZLIB}")
message(DEBUG "TACTILE_ENABLE_ZSTD: ${TACTILE_ENABLE_ZSTD}")
message(DEBUG "TACTILE_ENABLE_YAML: ${TACTILE_ENABLE_YAML}")
message(DEBUG "TACTILE_ENABLE_TMJ: ${TACTILE_ENABLE_TMJ}")
message(DEBUG "TACTILE_ENABLE_TMX: ${TACTILE_ENABLE_TMX}")
message(DEBUG "TACTILE_ENABLE_TSCN: ${TACTILE_ENABLE_TSCN}")
message(DEBUG "TACTILE_ENABLE_LTO: ${TACTILE_ENABLE_LTO}")

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

project(tactile CXX)

include("tools/cmake/common.cmake")

include("tools/cmake/targets/tactile_core.cmake")
if (TACTILE_ENABLE_TESTS)
  include("tools/cmake/targets/tactile_core_tests.cmake")
endif ()

if (TACTILE_ENABLE_ZLIB)
  include("tools/cmake/targets/tactile_zlib.cmake")
  if (TACTILE_ENABLE_TESTS)
    include("tools/cmake/targets/tactile_zlib_tests.cmake")
  endif ()
endif ()

if (TACTILE_ENABLE_EDITOR)
  include("tools/cmake/targets/tactile_editor.cmake")
  include("tools/cmake/targets/tactile.cmake")

  if (TACTILE_ENABLE_TESTS)
    include("tools/cmake/targets/tactile_editor_tests.cmake")
  endif ()
endif ()
