project(tactile-zstd-compression-test CXX)

add_executable(tactile-zstd-compression-test)

target_sources(tactile-zstd-compression-test
               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/main.cpp"
               "${PROJECT_SOURCE_DIR}/src/zstd_compressor_test.cpp"
               )

tactile_prepare_target(tactile-zstd-compression-test)

target_link_libraries(tactile-zstd-compression-test
                      PUBLIC
                      tactile::zstd_compression
                      GTest::gtest
                      )
