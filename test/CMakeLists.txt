cmake_minimum_required(VERSION 3.18)

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

    unit/core/properties/property_test.cpp

    unit/io/parsing/map_parser_test.cpp
    unit/io/parsing/parse_map_with_embedded_tilesets_test.cpp
    unit/io/parsing/parse_map_with_external_tilesets_test.cpp
    unit/io/parsing/parse_tmx_with_tile_nodes_test.cpp
    unit/io/parsing/to_map_document_test.cpp

    unit/utils/color_utils_test.cpp
    unit/utils/from_string_test.cpp
    )

add_executable(TactileTests ${TACTILE_TEST_FILES})

add_definitions(-DCENTURION_NO_SDL_MIXER)

target_include_directories(TactileTests
    PUBLIC unit
    PUBLIC ${TACTILE_ROOT_DIR}/src
    SYSTEM PUBLIC ${gtest_SOURCE_DIR}/include
    SYSTEM PUBLIC ${gtest_SOURCE_DIR}
    SYSTEM PUBLIC ${gmock_SOURCE_DIR}/include
    SYSTEM PUBLIC ${gmock_SOURCE_DIR})

target_link_libraries(TactileTests
    PUBLIC libtactile
    PUBLIC gtest
    PUBLIC gmock
    )

if (WIN32)
  copy_directory_post_build(TactileTests
      ${TACTILE_ROOT_DIR}/bin
      ${CMAKE_CURRENT_BINARY_DIR})
endif ()

copy_directory_post_build(TactileTests
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources)