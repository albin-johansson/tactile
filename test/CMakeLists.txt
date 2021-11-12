cmake_minimum_required(VERSION 3.16)

project(tactile-test CXX)

enable_testing()

find_package(GTest CONFIG REQUIRED)

set(TEST_FILES
    unit-tests/gtest_macros.hpp
    unit-tests/tactile_stdlib_test.cpp
    unit-tests/test_main.cpp

    unit-tests/core/commands/command_stack_test.cpp

    unit-tests/core/map/map_position_test.cpp

    unit-tests/core/properties/property_value_test.cpp

    unit-tests/core/systems/layer_system_test.cpp
    unit-tests/core/systems/registry_factory_system_test.cpp
    unit-tests/core/systems/tree_system_test.cpp

    unit-tests/io/parsing/map_parser_test.cpp
    unit-tests/io/parsing/parse_map_with_embedded_tilesets_test.cpp
    unit-tests/io/parsing/parse_map_with_external_tilesets_test.cpp
    unit-tests/io/parsing/parse_tmx_with_tile_nodes_test.cpp
    unit-tests/io/parsing/create_document_from_ir_test.cpp

    unit-tests/tactile-io/io_test_helpers.hpp
    unit-tests/tactile-io/ir_property_test.cpp
    unit-tests/tactile-io/parse_json_map_embedded_test.cpp
    unit-tests/tactile-io/parse_yaml_map_test.cpp

    unit-tests/utils/buffer_utils_test.cpp
    unit-tests/utils/color_utils_test.cpp
    )

add_executable(${TACTILE_TEST} ${TEST_FILES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${TACTILE_TEST}
      PRIVATE
      /wd4834  # "discarding return value of function with 'nodiscard' attribute"
      )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${TACTILE_TEST}
      PRIVATE
      -Wno-unused-result
      )
endif ()

target_include_directories(${TACTILE_TEST} PUBLIC unit)

target_link_libraries(${TACTILE_TEST}
    PUBLIC
    ${TACTILE_LIB}
    SDL2::SDL2main
    GTest::gtest
    )

target_precompile_headers(${TACTILE_TEST} REUSE_FROM ${TACTILE_LIB})

add_test(NAME ${TACTILE_TEST} COMMAND ${TACTILE_TEST})

if (WIN32)
  add_custom_command(TARGET ${TACTILE_TEST} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TactileIO.dll
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      )
endif ()

copy_directory_post_build(${TACTILE_TEST}
    ${CMAKE_CURRENT_SOURCE_DIR}/test-resources
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-resources)