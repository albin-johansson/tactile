project(tactile_zlib_tests_project CXX)

add_executable(tactile_zlib_tests)

target_sources(tactile_zlib_tests
               PRIVATE
               "tactile_zlib_tests.main.cpp"
               "zlib_compression_format.test.cpp"
               )

tactile_set_target_properties(tactile_zlib_tests)

target_link_libraries(tactile_zlib_tests
                      PRIVATE
                      tactile_interface_target
                      tactile_core
                      tactile_zlib
                      GTest::gtest
                      )
