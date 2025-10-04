# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

cmake_minimum_required(VERSION 4.0)

option(TACTILE_ENABLE_TESTS "Enable tests" "OFF")
option(TACTILE_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" "OFF")

message(DEBUG "TACTILE_ENABLE_TESTS: '${TACTILE_ENABLE_TESTS}'")
message(DEBUG "TACTILE_ENABLE_UBSAN: '${TACTILE_ENABLE_UBSAN}'")

project(tactile CXX C)

include("tools/cmake/common.cmake")
include("tools/cmake/dependencies.cmake")

include("tools/cmake/targets/dear_imgui.cmake")
include("tools/cmake/targets/tactile_core.cmake")
include("tools/cmake/targets/tactile_editor.cmake")
include("tools/cmake/targets/tactile.cmake")

if (TACTILE_ENABLE_TESTS)
  include("tools/cmake/targets/tactile_tests.cmake")
endif ()
