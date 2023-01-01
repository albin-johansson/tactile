cmake_minimum_required(VERSION 3.16)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

message("[Tactile]: CMake toolchain file is... ${CMAKE_TOOLCHAIN_FILE}")
message("[Tactile]: Vcpkg target triplet is... ${VCPKG_TARGET_TRIPLET}")

project(tactile
        HOMEPAGE_URL "https://github.com/albin-johansson/tactile"
        VERSION 0.5.0
        LANGUAGES CXX)

message("[Tactile]: Using C++${CMAKE_CXX_STANDARD}")
message("[Tactile]: Compiler is ${CMAKE_CXX_COMPILER_ID}")
message("[Tactile]: Current version is ${CMAKE_PROJECT_VERSION}")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  message("[Tactile]: Enabling IPO...")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif ()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRARY_DIR "${ROOT_DIR}/vendor")
set(RESOURCE_DIR "${ROOT_DIR}/assets")

set(TACTILE_APP_DIR "${PROJECT_SOURCE_DIR}/source/app")
set(TACTILE_LIB_DIR "${PROJECT_SOURCE_DIR}/source/lib")

# Target names
set(TACTILE_PROTO TactileProto)
set(TACTILE_LIB TactileLib)
set(TACTILE_EXE Tactile)
set(TACTILE_TEST TactileTests)

set(Boost_NO_WARN_NEW_VERSIONS ON)

include(cmake/tactile.cmake)

# System dependencies
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)

# Vcpkg dependencies
find_package(argparse CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
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

option(TACTILE_BUILD_APP_BUNDLE "Build the editor as a macOS application bundle (.app)" OFF)
option(TACTILE_BUILD_WITH_ASAN "Build with AddressSanitizer" OFF)

if (TACTILE_BUILD_WITH_ASAN MATCHES ON)
  tactile_enable_address_sanitizer()
endif ()

add_subdirectory(source/proto)
add_subdirectory(source/lib)
add_subdirectory(source/app)
add_subdirectory(test)