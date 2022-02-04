cmake_minimum_required(VERSION 3.16)

project(tactile-test CXX)

enable_testing()

find_package(GTest CONFIG REQUIRED)

set(TEST_FILES
    unit-tests/test_main.cpp

    unit-tests/app/core/attribute_value_test.cpp
    unit-tests/app/core/tile_position_test.cpp

    unit-tests/app/core/algorithms/flood_fill_test.cpp
    unit-tests/app/core/algorithms/invoke_n_test.cpp

    unit-tests/app/core/systems/component_system_test.cpp
    unit-tests/app/core/systems/layer_system_test.cpp
    unit-tests/app/core/systems/map_system_test.cpp
    unit-tests/app/core/systems/property_system_test.cpp
    unit-tests/app/core/systems/registry_system_test.cpp
    unit-tests/app/core/systems/tree_system_test.cpp
    unit-tests/app/core/systems/viewport_system_test.cpp

    unit-tests/app/core/utils/buffers_test.cpp
    unit-tests/app/core/utils/colors_test.cpp
    unit-tests/app/core/utils/formatted_string_test.cpp
    unit-tests/app/core/utils/strings_test.cpp
    unit-tests/app/core/utils/tiles_test.cpp
    unit-tests/app/core/utils/vector_map_test.cpp

    unit-tests/app/editor/commands/command_stack_test.cpp

    unit-tests/app/io/maps/map_parser_test.cpp
    unit-tests/app/io/maps/restore_document_from_ir_test.cpp
    unit-tests/app/io/maps/round_trip_test.cpp

    unit-tests/app/io/persistence/preference_state_test.cpp
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

copy_directory_post_build(${TACTILE_TEST}
                          ${CMAKE_CURRENT_SOURCE_DIR}/test-resources
                          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-resources)