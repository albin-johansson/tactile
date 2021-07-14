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

    unit/core/commands/command_stack_test.cpp

    unit/core/map/map_position_test.cpp
    unit/core/map/map_test.cpp

    unit/core/map/layers/layer_test.cpp
    unit/core/map/layers/object_layer_test.cpp
    unit/core/map/layers/object_test.cpp
    unit/core/map/layers/tile_layer_test.cpp

    unit/core/properties/property_context_test.cpp
    unit/core/properties/property_test.cpp

    unit/core/tileset/tileset_manager_test.cpp
    unit/core/tileset/tileset_test.cpp

    unit/io/parsing/map_parser_test.cpp
    unit/io/parsing/parse_map_with_embedded_tilesets_test.cpp
    unit/io/parsing/parse_map_with_external_tilesets_test.cpp
    unit/io/parsing/to_map_document_test.cpp

    )

add_executable(testTactile ${TACTILE_TEST_FILES})

add_definitions(-DCENTURION_NO_SDL_TTF)
add_definitions(-DCENTURION_NO_SDL_MIXER)

target_include_directories(testTactile
    PUBLIC unit
    PUBLIC ${TACTILE_ROOT_DIR}/src
    SYSTEM PUBLIC ${gtest_SOURCE_DIR}/include
    SYSTEM PUBLIC ${gtest_SOURCE_DIR}
    SYSTEM PUBLIC ${gmock_SOURCE_DIR}/include
    SYSTEM PUBLIC ${gmock_SOURCE_DIR})

target_link_libraries(testTactile
    PUBLIC libTactile
    PUBLIC gtest
    PUBLIC gmock
    )

copy_directory_post_build(testTactile
    ${TACTILE_ROOT_DIR}/bin
    ${CMAKE_CURRENT_BINARY_DIR})

copy_directory_post_build(testTactile
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources)