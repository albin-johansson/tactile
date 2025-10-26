# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

cmake_minimum_required(VERSION 3.31)

option(TACTILE_ENABLE_TESTS "Enable tests" "OFF")
option(TACTILE_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" "OFF")

message(DEBUG "TACTILE_ENABLE_TESTS: '${TACTILE_ENABLE_TESTS}'")
message(DEBUG "TACTILE_ENABLE_UBSAN: '${TACTILE_ENABLE_UBSAN}'")

project(tactile CXX C)

message(DEBUG "CMAKE_MACOSX_RPATH: ${CMAKE_MACOSX_RPATH}")
message(DEBUG "CMAKE_SKIP_RPATH: ${CMAKE_SKIP_RPATH}")
message(DEBUG "CMAKE_SKIP_BUILD_RPATH: ${CMAKE_SKIP_BUILD_RPATH}")
message(DEBUG "CMAKE_SKIP_INSTALL_RPATH: ${CMAKE_SKIP_INSTALL_RPATH}")
message(DEBUG "CMAKE_BUILD_WITH_INSTALL_RPATH: ${CMAKE_BUILD_WITH_INSTALL_RPATH}")
message(DEBUG "CMAKE_INSTALL_REMOVE_ENVIRONMENT_RPATH: ${CMAKE_INSTALL_REMOVE_ENVIRONMENT_RPATH}")
message(DEBUG "CMAKE_INSTALL_RPATH_USE_LINK_PATH: ${CMAKE_INSTALL_RPATH_USE_LINK_PATH}")
message(DEBUG "CMAKE_INSTALL_RPATH: ${CMAKE_INSTALL_RPATH}")

include("tools/cmake/common.cmake")
include("tools/cmake/dependencies.cmake")

include("tools/cmake/targets/dear_imgui.cmake")
include("tools/cmake/targets/tactile_core.cmake")
include("tools/cmake/targets/tactile_editor.cmake")
include("tools/cmake/targets/tactile.cmake")

if (TACTILE_ENABLE_TESTS)
  include("tools/cmake/targets/tactile_tests.cmake")
endif ()

# Place runtime artifacts, such as DLLs, next to the executables.
install(IMPORTED_RUNTIME_ARTIFACTS SDL3-shared DESTINATION "${PROJECT_BINARY_DIR}")
