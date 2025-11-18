# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

add_executable(tactile_tests)

tactile_init_target(tactile_tests)

target_link_libraries(tactile_tests
                      PUBLIC
                      tactile_core
                      tactile_editor
                      GTest::gtest
                      GTest::gmock
                      )

target_sources(tactile_tests
               PRIVATE
               "${PROJECT_SOURCE_DIR}/tests/core/container/hash_map.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/container/lookup.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/layer/layer.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/math/vec.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/meta/attribute.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/meta/attribute_set.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/util/defer.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/util/format_buffer.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/util/hash.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/casts.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/core/concepts.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/editor/command/command_stack.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/editor/event/event_queue.test.cpp"
               "${PROJECT_SOURCE_DIR}/tests/tactile_tests.main.cpp"
               )
