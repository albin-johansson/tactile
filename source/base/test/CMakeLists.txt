project(tactile-base-test CXX)

add_executable(tactile-base-test)

target_sources(tactile-base-test
               PRIVATE
               "src/io/color_parser_test.cpp"
               "src/io/int_parser_test.cpp"
               "src/io/tile_io_test.cpp"
               "src/meta/attribute_test.cpp"
               "src/numeric/saturate_cast_test.cpp"
               "src/numeric/vec_common_test.cpp"
               "src/numeric/vec_format_test.cpp"
               "src/numeric/vec_stream_test.cpp"
               "src/numeric/vec_test.cpp"
               "src/platform/bits_test.cpp"
               "src/platform/filesystem_test.cpp"
               "src/util/buffer_test.cpp"
               "src/util/format_test.cpp"
               "src/util/matrix_extent_test.cpp"
               "src/util/matrix_index_test.cpp"
               "src/util/tile_matrix_test.cpp"
               "src/main.cpp"
               )

tactile_prepare_target(tactile-base-test)

target_link_libraries(tactile-base-test
                      PRIVATE
                      tactile::base
                      GTest::gtest
                      )
