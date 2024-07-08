project(tactile-base-test CXX)

add_executable(tactile-base-test)

target_sources(tactile-base-test
               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/numeric/saturate_cast_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/buffer_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/format_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/main.cpp"
               )

tactile_prepare_target(tactile-base-test)

target_link_libraries(tactile-base-test
                      PRIVATE
                      tactile::base
                      GTest::gtest
                      )
