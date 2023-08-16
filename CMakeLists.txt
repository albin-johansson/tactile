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
option(TACTILE_ENABLE_TESTS "Build the test suites" OFF)

message(DEBUG "TACTILE_BUILD_APP_BUNDLE: ${TACTILE_BUILD_APP_BUNDLE}")
message(DEBUG "TACTILE_USE_SANITIZERS: ${TACTILE_USE_SANITIZERS}")
message(DEBUG "TACTILE_USE_LTO: ${TACTILE_USE_LTO}")
message(DEBUG "TACTILE_TREAT_WARNINGS_AS_ERRORS: ${TACTILE_TREAT_WARNINGS_AS_ERRORS}")
message(DEBUG "TACTILE_ENABLE_TESTS: ${TACTILE_ENABLE_TESTS}")

set(TACTILE_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(TACTILE_VENDOR_DIR "${TACTILE_ROOT_DIR}/vendor")
set(TACTILE_ASSETS_DIR "${TACTILE_ROOT_DIR}/assets")
set(TACTILE_INCLUDE_DIR "${TACTILE_ROOT_DIR}/source/public")
set(TACTILE_DEBUG_BUILD_DIR "${TACTILE_ROOT_DIR}/build/debug")
set(TACTILE_RELEASE_BUILD_DIR "${TACTILE_ROOT_DIR}/build/release")

message(DEBUG "TACTILE_ROOT_DIR: ${TACTILE_ROOT_DIR}")
message(DEBUG "TACTILE_VENDOR_DIR: ${TACTILE_VENDOR_DIR}")
message(DEBUG "TACTILE_ASSETS_DIR: ${TACTILE_ASSETS_DIR}")
message(DEBUG "TACTILE_INCLUDE_DIR: ${TACTILE_INCLUDE_DIR}")
message(DEBUG "TACTILE_DEBUG_BUILD_DIR: ${TACTILE_DEBUG_BUILD_DIR}")
message(DEBUG "TACTILE_RELEASE_BUILD_DIR: ${TACTILE_RELEASE_BUILD_DIR}")

# Target names
set(TACTILE_PROTO TactileProto)
set(TACTILE_LIB TactileLib) # TODO remove
set(TACTILE_EXE Tactile)
set(TACTILE_TEST TactileTests) # TODO remove

set(TACTILE_CORE tactile-core)
set(TACTILE_EDITOR tactile-editor)
set(TACTILE_APP tactile-app)
set(TACTILE_OPENGL_RHI tactile-opengl-rhi)
set(TACTILE_VULKAN_RHI tactile-vulkan-rhi)
set(TACTILE_YML_FORMAT tactile-yml-format)
set(TACTILE_TMJ_FORMAT tactile-tmj-format)
set(TACTILE_TMX_FORMAT tactile-tmx-format)

include(cmake/tactile.cmake)

# System dependencies
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

set(VULKAN_INCLUDE_DIR "$ENV{VULKAN_SDK}/include")
message(DEBUG "VULKAN_INCLUDE_DIR: ${VULKAN_INCLUDE_DIR}")

# Core dependencies
find_package(Boost REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(zstd CONFIG REQUIRED)

# Editor dependencies
find_package(imgui CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)

find_package(argparse CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)

find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_subdirectory(source/proto)
add_subdirectory(source/private)
add_subdirectory(source/lib)

add_subdirectory(source/exe)

if (TACTILE_ENABLE_TESTS MATCHES ON)
  find_package(doctest CONFIG REQUIRED)
  find_package(GTest CONFIG REQUIRED)

  add_subdirectory(test)
  add_subdirectory(tests/core)
endif ()
