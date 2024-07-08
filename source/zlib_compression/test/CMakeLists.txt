project(tactile-zlib-compression-test CXX)

add_executable(tactile-zlib-compression-test)

target_sources(tactile-zlib-compression-test
               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/main.cpp"
               "${PROJECT_SOURCE_DIR}/src/zlib_compressor_test.cpp"
               )

tactile_prepare_target(tactile-zlib-compression-test)

target_link_libraries(tactile-zlib-compression-test
                      PUBLIC
                      tactile::zlib_compression
                      GTest::gtest
                      )
