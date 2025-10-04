# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

include("FetchContent")

# These are some relevant options that can configured to optimize the local development experience.
# See https://cmake.org/cmake/help/latest/module/FetchContent.html
message(DEBUG "FETCHCONTENT_BASE_DIR: '${FETCHCONTENT_BASE_DIR}'")
message(DEBUG "FETCHCONTENT_QUIET: '${FETCHCONTENT_QUIET}'")
message(DEBUG "FETCHCONTENT_TRY_FIND_PACKAGE_MODE: '${FETCHCONTENT_TRY_FIND_PACKAGE_MODE}'")
message(DEBUG "FETCHCONTENT_FULLY_DISCONNECTED: '${FETCHCONTENT_FULLY_DISCONNECTED}'")
message(DEBUG "FETCHCONTENT_UPDATES_DISCONNECTED: '${FETCHCONTENT_UPDATES_DISCONNECTED}'")

# SDL 3.2.22
FetchContent_Declare("ext-SDL"
                     GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
                     GIT_TAG "a96677bdf6b4acb84af4ec294e5f60a4e8cbbe03"
                     GIT_SHALLOW "ON"
                     )

# Dear ImGui 1.92.3-docking
FetchContent_Declare("ext-dear_imgui"
                     GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
                     GIT_TAG "62275e877a691bd2e51d1ca86bdfed49fa5cb297"
                     GIT_SHALLOW "ON"
                     )

# GoogleTest 1.17.0
FetchContent_Declare("ext-googletest"
                     GIT_REPOSITORY "https://github.com/google/googletest.git"
                     GIT_TAG "52eb8108c5bdec04579160ae17225d66034bd723"
                     GIT_SHALLOW "ON"
                     )

FetchContent_MakeAvailable("ext-SDL"
                           "ext-dear_imgui"
                           )

if (TACTILE_ENABLE_TESTS)
  set(BUILD_GMOCK ON)
  set(INSTALL_GTEST OFF)
  set(GTEST_HAS_ABSL OFF)
  FetchContent_MakeAvailable("ext-googletest")
endif ()
