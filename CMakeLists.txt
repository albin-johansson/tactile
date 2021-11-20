cmake_minimum_required(VERSION 3.16)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

message("[Tactile]: CMake toolchain file is... ${CMAKE_TOOLCHAIN_FILE}")
message("[Tactile]: Vcpkg target triplet is... ${VCPKG_TARGET_TRIPLET}")

project(tactile CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRARY_DIR ${ROOT_DIR}/lib)
set(RESOURCE_DIR ${ROOT_DIR}/resources)

# Target names
set(TACTILE_PROTO TactileProto)
set(TACTILE_IO TactileIO)
set(TACTILE_LIB TactileCore)
set(TACTILE_EXE Tactile)
set(TACTILE_TEST TactileTests)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TACTILE_IO_IMPORT_LIBRARY "${CMAKE_BINARY_DIR}/modules/tactile-io/TactileIO.lib")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(TACTILE_IO_IMPORT_LIBRARY "${CMAKE_BINARY_DIR}/modules/tactile-io/libTactileIO.so")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
  set(TACTILE_IO_IMPORT_LIBRARY "${CMAKE_BINARY_DIR}/modules/tactile-io/libTactileIO.dylib")
else ()
  message(FATAL_ERROR "[Tactile]: Unsupported platform: ${CMAKE_SYSTEM_NAME}!")
endif ()

find_package(SDL2 CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

include(Utilities)

add_subdirectory(modules/tactile-proto)
add_subdirectory(modules/tactile-io)
add_subdirectory(modules/tactile)
add_subdirectory(test)