cmake_minimum_required(VERSION 3.16)

project(tactile-test CXX)

enable_testing()

find_package(GTest CONFIG REQUIRED)

set(TEST_FILES
    unit-tests/test_main.cpp

    unit-tests/tactile/core/attribute_value_test.cpp
    unit-tests/tactile/core/tile_position_test.cpp

    unit-tests/tactile/core/algorithms/flood_fill_test.cpp

    unit-tests/tactile/core/systems/component_system_test.cpp
    unit-tests/tactile/core/systems/layer_system_test.cpp
    unit-tests/tactile/core/systems/map_system_test.cpp
    unit-tests/tactile/core/systems/property_system_test.cpp
    unit-tests/tactile/core/systems/registry_system_test.cpp
    unit-tests/tactile/core/systems/tree_system_test.cpp
    unit-tests/tactile/core/systems/viewport_system_test.cpp

    unit-tests/tactile/core/utils/buffer_utils_test.cpp
    unit-tests/tactile/core/utils/color_utils_test.cpp
    unit-tests/tactile/core/utils/vector_map_test.cpp

    unit-tests/tactile/editor/commands/command_stack_test.cpp

    unit-tests/tactile/parsing/create_document_from_ir_test.cpp

    unit-tests/tactile-base/tactile_stdlib_test.cpp

    unit-tests/tactile-io/io_test_helpers.hpp
    unit-tests/tactile-io/ir_layer_test.cpp
    unit-tests/tactile-io/ir_object_test.cpp
    unit-tests/tactile-io/ir_property_test.cpp
    unit-tests/tactile-io/map_parser_test.cpp
    unit-tests/tactile-io/parse_json_map_embedded_test.cpp
    unit-tests/tactile-io/parse_map_with_embedded_tilesets_test.cpp
    unit-tests/tactile-io/parse_map_with_external_tilesets_test.cpp
    unit-tests/tactile-io/parse_tmx_with_tile_nodes_test.cpp
    unit-tests/tactile-io/parse_yaml_map_test.cpp
    )

add_executable(${TACTILE_TEST} ${TEST_FILES})

tactile_set_compile_options(${TACTILE_TEST})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${TACTILE_TEST}
                         PRIVATE
                         /wd4834  # disable [[nodiscard]] warnings
                         )

elseif (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|GNU")
  target_compile_options(${TACTILE_TEST}
                         PRIVATE
                         -Wno-unused-result
                         )
endif ()

target_include_directories(${TACTILE_TEST} PRIVATE unit)

target_link_libraries(${TACTILE_TEST}
                      PRIVATE
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