project(tactile_module_tests CXX)

find_package(GTest CONFIG REQUIRED)

add_executable(tactile_tests)

tactile_prepare_target(tactile_tests)

target_sources(tactile_tests
               PRIVATE
               "core/error.test.cpp"
               "core/strings.test.cpp"
               "io/base64.test.cpp"
               "meta/attr.test.cpp"
               "numeric/casts.test.cpp"
               "numeric/checked.test.cpp"
               "numeric/vec.test.cpp"
               "tactile_tests.main.cpp"
               )

target_link_libraries(tactile_tests
                      PRIVATE
                      tactile
                      GTest::gtest
                      )
