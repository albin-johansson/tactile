# Copyright (C) 2025 Albin Johansson

find_package(GTest CONFIG REQUIRED)

add_executable(tactile_zlib_tests)

tactile_set_target_properties(tactile_zlib_tests)

target_sources(tactile_zlib_tests
               PRIVATE
               "${TACTILE_TEST_DIR}/tactile/zlib/tactile_zlib_tests.main.cpp"
               "${TACTILE_TEST_DIR}/tactile/zlib/zlib_compression_format.test.cpp"
               )

target_link_libraries(tactile_zlib_tests
                      PRIVATE
                      tactile_interface_target
                      tactile_core
                      tactile_zlib
                      GTest::gtest
                      )
