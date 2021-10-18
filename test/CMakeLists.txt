cmake_minimum_required(VERSION 3.18)

project(tactile-test CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/test")

enable_testing()

set(TEST_FILES
    unit/gtest_macros.hpp
    unit/test_main.cpp

    unit/core/commands/command_stack_test.cpp

    unit/core/map/map_position_test.cpp

    unit/core/properties/property_value_test.cpp

    unit/core/systems/layer_system_test.cpp
    unit/core/systems/registry_factory_system_test.cpp
    unit/core/systems/tree_system_test.cpp

    unit/io/parsing/map_parser_test.cpp
    unit/io/parsing/parse_map_with_embedded_tilesets_test.cpp
    unit/io/parsing/parse_map_with_external_tilesets_test.cpp
    unit/io/parsing/parse_tmx_with_tile_nodes_test.cpp
    unit/io/parsing/to_map_document_test.cpp

    unit/utils/buffer_utils_test.cpp
    unit/utils/color_utils_test.cpp
    unit/utils/from_string_test.cpp
    )

add_executable(${TACTILE_TEST} ${TEST_FILES})
add_dependencies(${TACTILE_TEST} ${TACTILE_LIB})

target_include_directories(${TACTILE_TEST}
    PUBLIC
    unit

    SYSTEM PUBLIC
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
    ${gmock_SOURCE_DIR}/include
    ${gmock_SOURCE_DIR})

target_link_libraries(${TACTILE_TEST}
    PUBLIC
    ${TACTILE_LIB}
    gtest
    gmock)

target_precompile_headers(${TACTILE_TEST} REUSE_FROM ${TACTILE_LIB})

if (WIN32)
  copy_directory_post_build(${TACTILE_TEST} ${ROOT_DIR}/bin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  add_custom_command(TARGET ${TACTILE_TEST} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../TactileIO.dll
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      )
endif ()

copy_directory_post_build(${TACTILE_TEST}
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)