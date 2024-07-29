project(tactile-tiled-tmj-format-test CXX)

add_executable(tactile-tiled-tmj-format-test)

target_sources(tactile-tiled-tmj-format-test
               PRIVATE
               "src/main.cpp"
               "src/tmj_format_attribute_parser_test.cpp"
               "src/tmj_format_layer_emitter_test.cpp"
               "src/tmj_format_layer_parser_test.cpp"
               "src/tmj_format_map_parser_test.cpp"
               "src/tmj_format_map_emitter_test.cpp"
               "src/tmj_format_meta_emitter_test.cpp"
               "src/tmj_format_object_emitter_test.cpp"
               "src/tmj_format_object_parser_test.cpp"
               "src/tmj_format_tile_emitter_test.cpp"
               "src/tmj_format_tileset_emitter_test.cpp"
               "src/tmj_format_tileset_parser_test.cpp"
               )

tactile_prepare_target(tactile-tiled-tmj-format-test)

target_link_libraries(tactile-tiled-tmj-format-test
                      PRIVATE
                      tactile::tiled_tmj_format
                      tactile::base_test_util
                      tactile::null_renderer
                      $<$<BOOL:TACTILE_BUILD_ZLIB_COMPRESSION>:tactile::zlib_compression>
                      $<$<BOOL:TACTILE_BUILD_ZSTD_COMPRESSION>:tactile::zstd_compression>
                      GTest::gtest
                      )

target_compile_definitions(tactile-tiled-tmj-format-test
                           PRIVATE
                           "$<$<BOOL:TACTILE_BUILD_ZLIB_COMPRESSION>:TACTILE_HAS_ZLIB_COMPRESSION>"
                           "$<$<BOOL:TACTILE_BUILD_ZSTD_COMPRESSION>:TACTILE_HAS_ZSTD_COMPRESSION>"
                           )
