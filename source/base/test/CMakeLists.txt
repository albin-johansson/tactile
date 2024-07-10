project(tactile-base-test CXX)

add_executable(tactile-base-test)

target_sources(tactile-base-test
               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/meta/attribute_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/numeric/saturate_cast_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/numeric/vec_common_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/numeric/vec_format_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/numeric/vec_stream_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/numeric/vec_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/buffer_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/format_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/matrix_extent_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/matrix_index_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/main.cpp"
               )

tactile_prepare_target(tactile-base-test)

target_link_libraries(tactile-base-test
                      PRIVATE
                      tactile::base
                      GTest::gtest
                      )
