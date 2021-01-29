cmake_minimum_required(VERSION 3.12)

project(tactile-test CXX)

enable_testing()

include(DownloadProject)
include(CTest)

download_project(PROJ googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    "UPDATE_DISCONNECTED 1")

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

set(TACTILE_TEST_FILES
    unit/gtest_macros.hpp
    unit/test_main.cpp

    unit/core/flood_fill_test.cpp
    unit/core/map_test.cpp
    unit/core/position_test.cpp
    unit/core/tile_layer_test.cpp
    unit/core/tile_size_test.cpp
    unit/core/tileset_manager_test.cpp
    unit/core/tileset_test.cpp

    unit/core/map/layer/object_layer_test.cpp

    unit/core/property/property_delegate_test.cpp
    unit/core/property/property_test.cpp

    unit/core/viewmodel/property/property_items_test.cpp

    unit/parse/json_utils_test.cpp
    unit/parse/json_map_parser_test.cpp
    unit/parse/xml_map_parser_test.cpp
    unit/parse/xml_utils_test.cpp
    unit/parse/map_import_test.cpp

    unit/service/json_import_test.cpp

    unit/util/nenya_test.cpp
    unit/util/vector_map_test.cpp)

add_executable(testTactile ${TACTILE_TEST_FILES})

add_library(libCatch INTERFACE)
target_include_directories(libCatch INTERFACE catch)

target_include_directories(testTactile
    PUBLIC unit
    PUBLIC ${TACTILE_ROOT_DIR}/src
    PUBLIC ${TACTILE_ROOT_DIR}/src/core
    PUBLIC ${TACTILE_ROOT_DIR}/src/core/util
    PUBLIC ${TACTILE_ROOT_DIR}/src/core/tile
    SYSTEM PUBLIC ${gtest_SOURCE_DIR}/include
    SYSTEM PUBLIC ${gtest_SOURCE_DIR}
    SYSTEM PUBLIC ${gmock_SOURCE_DIR}/include
    SYSTEM PUBLIC ${gmock_SOURCE_DIR})

target_link_libraries(testTactile
    PUBLIC libTactile
    PUBLIC libCatch
    PUBLIC gtest
    PUBLIC gmock)

copy_directory_post_build(testTactile
    ${TACTILE_ROOT_DIR}/bin
    ${CMAKE_CURRENT_BINARY_DIR})

copy_directory_post_build(testTactile
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR})