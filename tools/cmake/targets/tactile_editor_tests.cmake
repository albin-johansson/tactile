# Copyright (C) 2025 Albin Johansson

find_package(GTest CONFIG REQUIRED)

add_executable(tactile_editor_tests)

tactile_set_target_properties(tactile_editor_tests)

target_sources(tactile_editor_tests
               PRIVATE
               "${TACTILE_TEST_DIR}/tactile/editor/cmd/command_stack.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/editor/tactile_editor_tests.main.cpp"
               )

target_link_libraries(tactile_editor_tests
                      PRIVATE
                      tactile_interface_target
                      tactile_editor
                      GTest::gtest
                      )
