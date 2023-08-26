cmake_minimum_required(VERSION 3.16)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  cmake_path(SET VCPKG_ROOT NORMALIZE $ENV{VCPKG_ROOT})
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

if (NOT CMAKE_CXX_STANDARD MATCHES "11|14|17|20|23")
  set(CMAKE_CXX_STANDARD 23)
endif ()

message(DEBUG "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(DEBUG "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(DEBUG "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

project(tactile
        HOMEPAGE_URL "https://github.com/albin-johansson/tactile"
        VERSION 0.5.0
        LANGUAGES CXX)

message(DEBUG "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(DEBUG "CMAKE_PROJECT_VERSION: ${CMAKE_PROJECT_VERSION}")

option(TACTILE_BUILD_APP_BUNDLE "Build the editor as a macOS application bundle (.app)" OFF)
option(TACTILE_BUILD_TESTS "Build the test suite" OFF)
option(TACTILE_USE_SANITIZERS "Enable runtime sanitizers" OFF)
option(TACTILE_USE_LTO "Enable link-time optimizations" OFF)

message(DEBUG "TACTILE_BUILD_APP_BUNDLE: ${TACTILE_BUILD_APP_BUNDLE}")
message(DEBUG "TACTILE_BUILD_TESTS: ${TACTILE_BUILD_TESTS}")
message(DEBUG "TACTILE_USE_SANITIZERS: ${TACTILE_USE_SANITIZERS}")
message(DEBUG "TACTILE_USE_LTO: ${TACTILE_USE_LTO}")

set(TACTILE_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(TACTILE_VENDOR_DIR "${TACTILE_ROOT_DIR}/vendor")
set(TACTILE_RESOURCE_DIR "${TACTILE_ROOT_DIR}/assets")
set(TACTILE_APP_DIR "${PROJECT_SOURCE_DIR}/source/app")
set(TACTILE_LIB_DIR "${PROJECT_SOURCE_DIR}/source/lib")

message(DEBUG "TACTILE_ROOT_DIR: ${TACTILE_ROOT_DIR}")
message(DEBUG "TACTILE_VENDOR_DIR: ${TACTILE_VENDOR_DIR}")
message(DEBUG "TACTILE_RESOURCE_DIR: ${TACTILE_RESOURCE_DIR}")
message(DEBUG "TACTILE_APP_DIR: ${TACTILE_APP_DIR}")
message(DEBUG "TACTILE_LIB_DIR: ${TACTILE_LIB_DIR}")

# Target names
set(TACTILE_PROTO TactileProto)
set(TACTILE_LIB TactileLib)
set(TACTILE_EXE Tactile)
set(TACTILE_TEST TactileTests)

find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)
find_package(argparse CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)

find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")

set(TACTILE_COMPILE_OPTIONS)
set(TACTILE_LINK_OPTIONS)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(TACTILE_COMPILE_OPTIONS
      /EHsc
      /MP
      /W4
      /bigobj
      /permissive-
      /Zc:preprocessor
      /Zc:__cplusplus
      )
else ()
  set(TACTILE_COMPILE_OPTIONS
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
      )

  if (TACTILE_BUILD_WITH_ASAN MATCHES ON)
    list(APPEND
         ${TACTILE_COMPILE_OPTIONS}
         -fno-omit-frame-pointer
         -fsanitize=address
         )
    list(APPEND
         ${TACTILE_LINK_OPTIONS}
         -fno-omit-frame-pointer
         -fsanitize=address)
  endif ()
endif ()

message(DEBUG "TACTILE_COMPILE_OPTIONS: ${TACTILE_COMPILE_OPTIONS}")
message(DEBUG "TACTILE_LINK_OPTIONS: ${TACTILE_LINK_OPTIONS}")

set(TACTILE_PRECOMPILED_HEADERS
    <algorithm>
    <array>
    <charconv>
    <chrono>
    <codecvt>
    <concepts>
    <cstddef>
    <cstdint>
    <cstring>
    <cstdlib>
    <ctime>
    <cmath>
    <deque>
    <exception>
    <filesystem>
    <fstream>
    <functional>
    <iterator>
    <limits>
    <locale>
    <map>
    <memory>
    <numbers>
    <optional>
    <ostream>
    <queue>
    <random>
    <span>
    <sstream>
    <string>
    <string_view>
    <system_error>
    <type_traits>
    <unordered_map>
    <utility>
    <variant>
    <vector>
    )

message(DEBUG "TACTILE_PRECOMPILED_HEADERS: ${TACTILE_PRECOMPILED_HEADERS}")

function(tactile_prepare_target target)
  set_target_properties(${target}
                        PROPERTIES
                        CXX_EXTENSIONS OFF
                        CXX_STANDARD_REQUIRED ON
                        INTERPROCEDURAL_OPTIMIZATION ${TACTILE_USE_LTO}
                        )

  target_compile_options(${target} PRIVATE ${TACTILE_COMPILE_OPTIONS})
  target_link_options(${target} PRIVATE ${TACTILE_LINK_OPTIONS})
  target_precompile_headers(${target} PRIVATE ${TACTILE_PRECOMPILED_HEADERS})
endfunction()

function(tactile_copy_directory_post_build target from to)
  add_custom_command(TARGET ${target} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${from}
                     ${to})
endfunction()

add_subdirectory(source/proto)
add_subdirectory(source/lib)
add_subdirectory(source/app)

if (TACTILE_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif ()
