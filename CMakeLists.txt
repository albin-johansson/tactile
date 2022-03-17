cmake_minimum_required(VERSION 3.16)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

message("[Tactile]: CMake toolchain file is... ${CMAKE_TOOLCHAIN_FILE}")
message("[Tactile]: Vcpkg target triplet is... ${VCPKG_TARGET_TRIPLET}")

project(tactile
        HOMEPAGE_URL "https://github.com/albin-johansson/tactile"
        VERSION 0.3.0
        LANGUAGES CXX)

message("[Tactile]: Current version is ${CMAKE_PROJECT_VERSION}")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  message("[Tactile]: Enabling IPO...")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif ()

message("[Tactile]: C++ compiler is... ${CMAKE_CXX_COMPILER_ID}")

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRARY_DIR ${ROOT_DIR}/lib)
set(RESOURCE_DIR ${ROOT_DIR}/resources)

# Target names
set(TACTILE_PROTO TactileProto)
set(TACTILE_LIB TactileCore)
set(TACTILE_EXE Tactile)
set(TACTILE_TEST TactileTests)

# These are not acquired using vcpkg
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)

find_package(GLEW CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2-image CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb.h")

include(Utilities)

function(tactile_set_compile_options target)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${target}
                           PRIVATE
                           /EHsc
                           /MP
                           /W4
                           /bigobj
                           /Zc:preprocessor
                           /Zc:__cplusplus
                           /wd5105  # "macro expansion producing 'defined' has undefined behavior"
                           )

  elseif (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|GNU")
    target_compile_options(${target}
                           PRIVATE
                           -Wall
                           -Wextra
                           -Wpedantic
                           -Wconversion
                           )
  endif ()
endfunction()

add_subdirectory(source/proto)
add_subdirectory(source/app)
add_subdirectory(test)