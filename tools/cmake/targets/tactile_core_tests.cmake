# Copyright (C) 2025 Albin Johansson

find_package(GTest CONFIG REQUIRED)

add_executable(tactile_core_tests)

tactile_set_target_properties(tactile_core_tests)

target_sources(tactile_core_tests
               PRIVATE
               "${TACTILE_TEST_DIR}/tactile/core/common/error.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/common/strings.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/io/base64.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/layer/annotation_layer.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/layer/group_layer.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/layer/layer.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/layer/layer_info.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/layer/tile_layer.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/meta/attr.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/numeric/casts.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/numeric/checked.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/numeric/vec.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/util/defer.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/util/validation.test.cpp"
               "${TACTILE_TEST_DIR}/tactile/core/tactile_core_tests.main.cpp"
               )

target_link_libraries(tactile_core_tests
                      PRIVATE
                      tactile_interface_target
                      tactile_core
                      GTest::gtest
                      )
