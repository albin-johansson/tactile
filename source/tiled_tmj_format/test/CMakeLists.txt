project(tactile-tiled-tmj-format-test CXX)

add_executable(tactile-tmj-format-test)

target_sources(tactile-tmj-format-test
               PRIVATE
               "src/main.cpp"
               "src/tmj_format_attribute_parser_test.cpp"
               "src/tmj_format_layer_parser_test.cpp"
               "src/tmj_format_object_parser_test.cpp"
               )

tactile_prepare_target(tactile-tmj-format-test)

target_link_libraries(tactile-tmj-format-test
                      PRIVATE
                      tactile::tiled_tmj_format
                      tactile::null_renderer
                      $<$<BOOL:TACTILE_BUILD_ZLIB_COMPRESSION>:tactile::zlib_compression>
                      $<$<BOOL:TACTILE_BUILD_ZSTD_COMPRESSION>:tactile::zstd_compression>
                      GTest::gtest
                      )

target_compile_definitions(tactile-tmj-format-test
                           PRIVATE
                           "$<$<BOOL:TACTILE_BUILD_ZLIB_COMPRESSION>:TACTILE_HAS_ZLIB_COMPRESSION>"
                           "$<$<BOOL:TACTILE_BUILD_ZSTD_COMPRESSION>:TACTILE_HAS_ZSTD_COMPRESSION>"
                           )