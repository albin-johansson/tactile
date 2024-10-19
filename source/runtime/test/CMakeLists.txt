project(tactile-runtime-test CXX)

add_executable(tactile-runtime-test)

target_sources(tactile-runtime-test
               PRIVATE
               "src/main.cpp"
               "src/save_format_roundtrip_test.cpp"
               )

tactile_prepare_target(tactile-runtime-test)

target_link_libraries(tactile-runtime-test
                      PRIVATE
                      tactile::test_util
                      tactile::null_renderer
                      $<$<BOOL:TACTILE_BUILD_TILED_TMJ_FORMAT>:tactile::tiled_tmj_format>
                      $<$<BOOL:TACTILE_BUILD_ZLIB_COMPRESSION>:tactile::zlib_compression>
                      $<$<BOOL:TACTILE_BUILD_ZSTD_COMPRESSION>:tactile::zstd_compression>
                      GTest::gtest
                      )

