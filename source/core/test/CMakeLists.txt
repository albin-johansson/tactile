project(tactile-core-test CXX)

add_executable(tactile_core_test)

tactile_prepare_target(tactile_core_test)

target_sources(tactile_core_test
               PRIVATE
               "src/main.cpp"
               )

target_link_libraries(tactile_core_test
                      PRIVATE
                      tactile_basic_target
                      tactile::core
                      GTest::gtest
                      )
