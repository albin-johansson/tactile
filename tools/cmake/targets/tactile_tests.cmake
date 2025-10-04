# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

add_executable(tactile_tests)

tactile_init_target(tactile_tests)

target_link_libraries(tactile_tests
                      PUBLIC
                      tactile_core
                      tactile_editor
                      GTest::gtest
                      )

target_sources(tactile_tests
               PRIVATE
               "${PROJECT_SOURCE_DIR}/tests/tactile_tests.main.cpp"
               )
